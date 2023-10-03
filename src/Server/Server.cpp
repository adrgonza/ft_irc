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
	if (poll(_pollFd, _clients.size() + 1, INT_MAX) < 0)
		return (std::cout << "Error: syscall poll failed.." << std::endl, false);

	if (_pollFd[0].revents == POLLIN)
	{
		std::cout << "Incomming connecction..." << std::endl;
		_connectionFd = accept(_socketFd, (struct sockaddr *)NULL, NULL);
		if (_connectionFd == -1)
			return (std::cout << "Error accepting client's connection" << std::endl, false);

		if (this->_clients.size() >= BACKLOG)
			return (std::cout << "Error: max connections limit reached" << std::endl, true);

		std::cout << "[SERVER]: A new connection has been made." << std::endl;

		Client newClient(_connectionFd);

		newClient.sendMessage("NOTICE AUTH :*** Checking Ident");

		_clients.push_back(newClient);


		for (size_t i = 1; i <= _clients.size(); i++) // Saves the new connection
		{
			if (_pollFd[i].fd != -1)
				continue;

			_pollFd[i].fd = _connectionFd;
			_pollFd[i].events = POLLIN;
			break;
		}
	}

	for (size_t i = 1; i <= _clients.size(); i++)
	{
		if (_pollFd[i].fd != -1 && _pollFd[i].revents & POLLIN && handleClientCommunications(i) == false)
			return (false);
	}
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
		std::cout << "[SERVER]: A Client was disconnected from the server" << std::endl;
		Client *delet = findClientByFd(_pollFd[i].fd);
		close(delet->getFd());
		for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			if (it->getFd() == delet->getFd())
			{
				_disconnectedClients.push_back(*delet);
				_clients.erase(it);
				break;
			}
		}
		_pollFd[i].fd = -1;
		_pollFd[i].revents = 0;
	}
	else
	{
		std::vector<Client>::iterator caller = getClientByFd(_pollFd[i].fd);
		if (caller == _clients.end())
		{
			std::cout << "[SERVER :: WARNING]: getClientByFd() failed before executing a command" << std::endl;
			return (true);
		}

		handleClientInput(*caller, buffer);
	}
	return (true);
}

bool Server::handleClientInput(Client &caller, std::string message)
{
	if (message.find("\r") == message.npos)
	{
		if (message.find("\n") != message.npos)
		{
			caller.sendMessage("You are a invalid Client!");
			return (true);
		}
		if (caller.getjoined().empty())
			caller.setjoined(message);
		else if (caller.getjoined().length() >= 1024)
			caller.sendMessage("Msg buffer is full!");
		else
			caller.setjoined(caller.getjoined() + message);

		return (true);
	}
	if (!caller.getjoined().empty())
	{
		message = caller.getjoined() + message;
		caller.setjoined("");
	}

	std::istringstream splitted(message);
	std::string command;
	splitted >> command;

	for(std::string::iterator it = command.begin(); it != command.end(); ++it)
		*it = std::toupper(*it);

	std::string body;
	std::getline(splitted >> std::ws, body);
	if (body.empty())
		body = IRC_ENDLINE;

	size_t endlinePosition = body.find("\r");
	if (endlinePosition != std::string::npos)
		body = body.substr(0, endlinePosition);
	else
	{
		caller.sendMessage("You are a invalid Client!");
		return (true);
	}
	if (caller.getKey() == true && !caller.getNickname().empty() && !caller.getUsername().empty())
		handleCommand(caller, command, body);
	else if (command == "PASS")
		checkPassword(body, caller);
	else if (command == "NICK")
		caller.changeNickname(_clients, body);
	else if (command == "USER")
		caller.changeUserName(body);
	else
	{
		if (caller.getNickname().empty() || caller.getUsername().empty())
			caller.sendMessage("NOTICE AUTH :*** Checking Ident");
		else if (caller.getKey() == false)
			caller.sendMessage(ERR_PASSWDREQUIRED, caller.getNickname().c_str());
	}

	if (caller.getKey() == true && !caller.getNickname().empty() && !caller.getUsername().empty() && caller.getFirsTime() == false)
	{
		caller.setFirstTime(true);
		caller.sendMessage(RPL_MOTDSTART, caller.getNickname().c_str(), "Welcome to the TONY_WARRIORS Internet Relay Chat Network");
	}
	return (true);
}

void Server::checkPassword(std::string body, Client &caller)
{
	if (body == _password)
		caller.giveKey(true);
	else
	{
		caller.sendMessage(ERR_PASSWDMISMATCH, caller.getNickname().c_str());
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
