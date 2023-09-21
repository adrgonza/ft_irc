#include "Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password) {}

Server::~Server() {}

bool Server::run()
{
	if ((_socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // 1-familia de direcciones(ipv4) 2-tipo de socket (tipo orientado a protocolo TCP) 3-protocolo (automatico, TCP)
		return (std::cout << "Error: could not create the sockets.." << std::endl, false);

	struct sockaddr_in serverAddress;
	bzero(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;					// specing the family, interenet (address)
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);	// responding to anything
	serverAddress.sin_port = htons(_port);				// convert server port nb to network standart byte order (to avoid to conections use different byte order)

	if (bind(_socketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // set the address wher is gonna be listening
		return (std::cout << "Error: could not bind.." << std::endl, false);

	if (listen(_socketFd, serverAddress.sin_port) < 0)
		return (std::cout << "Error: trying to listeng" << std::endl, false);

	std::cout << "Waiting for a connection in '127.0.0.1' port: " << _port << std::endl; // localhost ip default = 127.0.0.1

	fcntl(_socketFd, F_SETFL, O_NONBLOCK); // avoid system differences

	_pollFd[0].fd = _socketFd;
	_pollFd[0].events = POLLIN;
	_pollFd[0].revents = 0;

	for (int i = 1; i <= BACKLOG; i++)
		_pollFd[i].fd = -1;

	while (true)
		if (handleClientConnections() == false)
			return (false);
}

bool Server::handleClientConnections()
{
	if (poll(_pollFd, _clients.size() + 1, -1) < 0)
		return (std::cout << "Error: syscall poll failed.." << std::endl, false);

	if (_pollFd[0].revents == POLLIN)
	{
		std::cout << "Incomming connecction..." << std::endl;
		_connectionFd = accept(_socketFd, (struct sockaddr *) NULL, NULL);
		if (_connectionFd == -1)
			return (std::cout << "Error accepting client's connection" << std::endl, false);

		fcntl(_socketFd, F_SETFL, O_NONBLOCK); // avoid system differences

		if (this->_clients.size() >= BACKLOG)
			return (std::cout << "Error: max connections limit reached" << std::endl, true);

		std::cout << "[SERVER]: A new connection has been made." << std::endl;

		_clients.push_back(Client(_connectionFd));

		size_t i = 1;
		while (i <= _clients.size() && _pollFd[i].fd != -1)
			i++;
		_pollFd[i].fd = _connectionFd;
		_pollFd[i].events = POLLIN;
	}

	for (size_t i = 1; i <= _clients.size(); i++)
		if (_pollFd[i].fd != -1 && _pollFd[i].revents == POLLIN && handleClientCommunications(i) == false)
			return (false);

	return (true);
}

bool Server::handleClientCommunications(size_t i)
{
	char buffer[BUFFER_SIZE + 1];
	bzero(&buffer, sizeof(buffer));

	int readSize = read(_pollFd[i].fd, buffer, BUFFER_SIZE);
	if (readSize == -1)
		return (std::cout << "Error: dont have access to read client fd." << std::endl, false);

	if (readSize == 0)
	{
		//disconnect a client
		std::cout << "[SERVER]: A Client was disconnected from the server" << std::endl;
		close(_pollFd[i].fd);
		_clients.erase(std::find(_clients.begin(), _clients.end(), _pollFd[i].fd));

		for (size_t i = 0; i < BACKLOG; i++)
		{
			if (_pollFd[i + 1].fd == _pollFd[i].fd)
				_pollFd[i + 1].fd = -1;
			_pollFd[i].revents = 0;
		}
	}
	else
	{
		std::vector<Client>::iterator caller = getClientByFd(_pollFd[i].fd);
		if (caller == _clients.end())
		{
			std::cout << "[SERVER :: WARNING]: getClientByFd() failed before executing a command" << std::endl;
			return (true);
		}
		// TODO: Handle not-ended inputs (see subject)
		handleClientInput(*caller, buffer);
	}
	return (true);
}

bool Server::handleClientInput(Client &caller, std::string message)
{
	std::cout << "holaaa" <<std::endl;
	std::istringstream splitted(message);
	std::string command;
	splitted >> command;

	std::string body;
	std::getline(splitted >> std::ws, body);
	if (body.empty())
		body = IRC_ENDLINE;

	size_t endlinePosition = body.find("\r");
	if (endlinePosition != std::string::npos) // If the message does not end with '\r\n' should be ignored, but for now we accept it. TODO: change this
		body = body.substr(0, endlinePosition);

	if (command == "PASS")
		checkPassword(body, caller);

	if (caller.getKey() == true)
		handleCommand(caller, command, body);
	else
		std::cout << "Error: a password is required.." << std::endl;

	return (true);
}

void Server::checkPassword(std::string body, Client &caller)
{
	if (body == _password)
	{
		std::cout << "A client has been accepted.." << std::endl;
		caller.giveKey(true);
	}
	else
	{
		std::cout << "Error: invalid password.." << std::endl; //should send a message to client
		caller.giveKey(false);
	}
}

std::vector<Client>::iterator Server::getClientByFd(int fd)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->getFd() == fd)
			return it;
	}
	return _clients.end();
}
