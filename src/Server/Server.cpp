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

	std::cout << "Waiting for a connection in '127.0.0.1' port: " << _port << std::endl; // localhost ip default = 127.0.0.1

	if (listen(_socketFd, serverAddress.sin_port) < 0)
		return (std::cout << "Error: trying to listeng" << std::endl, false);

	// fcntl(_socketFd, F_SETFL, O_NONBLOCK); // avoid system differences

	_pollFd.fd = _socketFd;
	_pollFd.events = POLLIN;
	_pollFd.revents = 0;

	while (true)
		if (handleClientConnections() == false)
			return (false);
}

// std::vector<Client>::iterator Server::getClientByFd(int fd)
// {
// 	for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
// 	{
// 		if (it->getFd() == fd)
// 			return it;
// 	}
// 	return this->clients.end();
// }


bool Server::handleClientConnections()
{
	if (poll(&_pollFd, 1, -1) < 0)
		return (std::cout << "Error: syscall poll failed.." << std::endl, false);

	if (_pollFd.revents == POLLIN)
	{
		std::cout << "Incomming connecction" << std::endl;
		_connectionFd = accept(_socketFd, (struct sockaddr *) NULL, NULL);
		if (_connectionFd == -1)
			return (std::cout << "Error accepting client's connection" << std::endl, false);

		//fcntl(_SocketFd, F_SETFL, O_NONBLOCK); // avoid system differences

		handleClientCommunications();
	}
	return (true);
}

void Server::handleClientCommunications()
{
	for (size_t i = 1; i <= clients.size(); i++)
	{
		if (this->backlogFds[i].fd == -1)
			continue;

		if (this->backlogFds[i].revents & POLLIN)
		{
			char buffer[BUFFER_SIZE + 1];
			size_t readSize = read(this->backlogFds[i].fd, buffer, BUFFER_SIZE);
			if (readSize < 0)
				throw std::runtime_error("A Client Socket can't be read from");

			buffer[readSize] = '\0';

			if (readSize == 0)
				disconnectClient(this->backlogFds[i].fd);
			else
			{
				std::vector<Client>::iterator caller = getClientByFd(this->backlogFds[i].fd);
				if (caller == clients.end())
				{
					std::cout << "[SERVER :: WARNING]: getClientByFd() failed before executing a command" << std::endl;
					continue;
				}
				// TODO: Handle not-ended inputs (see subject)
				this->handleClientInput(*caller, buffer);
			}
		}
	}
}

void Server::disconnectClient(int clientFd)
{
	std::cout << "[SERVER]: A Client was disconnected from the server" << std::endl;
	// Closes the FD (AKA: closes the connection)
	close(clientFd);

	// Removes the client from the Vector of Clients
	for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		if (it->getFd() == clientFd)
		{
			clients.erase(it);
			break;
		}
	}

	// Remove the Client FD from the backlog
	for (size_t i = 0; i < BACKLOG; i++)
	{
		if (this->backlogFds[i + 1].fd == clientFd)
			this->backlogFds[i + 1].fd = -1;
		this->backlogFds[i].revents = 0;
	}
}

// server_utils.cpp
// void handleCommand(Client caller, std::string command, std::string body);

void Server::handleClientInput(Client &caller, std::string message)
{
	(void)caller;
	size_t spaceSeparator = message.find(' ');
	std::string command = (spaceSeparator == std::string::npos) ? message : message.substr(0, spaceSeparator);
	std::string body = (spaceSeparator == std::string::npos) ? IRC_ENDLINE : message.substr(spaceSeparator + 1);

	size_t endlinePosition = body.find(IRC_ENDLINE);
	if (endlinePosition != std::string::npos) // If the message does not end with '\r\n' should be ignored, but for now we accept it. TODO: change this
		body = body.substr(0, endlinePosition);

	handleCommand(caller, command, body);
}
