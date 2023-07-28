/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dangonza <dangonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 13:14:29 by dangonza          #+#    #+#             */
/*   Updated: 2023/07/28 15:41:50 by dangonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Server::Server() {}
Server::~Server() {}
Server::Server(const Server & obj) { *this = obj; }
Server& Server::operator=(const Server& obj) { (void)obj; return *this; }

// Server utils
std::vector<Client>::iterator Server::getClientByFd(int fd)
{
	for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		if (it->getFd() == fd)
			return it;
	}
	return this->clients.end();
}

// Execute the Server
void Server::run(int port)
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
		throw std::runtime_error("Server Socket creation failed");

	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress)); // Not really needed
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Any useful address on this machine
	serverAddress.sin_port = htons(port);

	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		throw std::runtime_error("Server Socket bind failed");

	if (listen(serverSocket, serverAddress.sin_port) < 0)
		throw std::runtime_error("Server Socket listen failed");

	this->backlogFds[0].fd = serverSocket;
	this->backlogFds[0].events = POLLIN;
	this->backlogFds[0].revents = 0;


	for (int i = 1; i <= BACKLOG; i++)
	{
		this->backlogFds[i].fd = -1;
	}

	// Inform everything is correct so far
	std::cout << "Server listening on port " << port << std::endl;

	// Accept the data from incoming sockets in a iterative way
	this->handleClientConnections(serverSocket);
	// Close connections
}

void Server::handleClientConnections(int socket)
{
	while (true)
	{
		if (poll(this->backlogFds, clients.size() + 1, -1) == -1)
			throw std::runtime_error("A call to poll() failed");

		int connectionFd = -1;
		struct sockaddr_in client;
		unsigned int clientLenght = sizeof(client);
		if (this->backlogFds[0].revents & POLLIN) // If poll() allows us to read & write ...
		{
			connectionFd = accept(socket, (struct sockaddr *)&client, &clientLenght); // Accepts incoming connections
			if (connectionFd < 0)
				throw std::runtime_error("Connection not accepted"); // TODO: add errno & strerror(errno)

			// Check if there is space for the new connection in the BackLog
			if (this->clients.size() >= BACKLOG)
				throw std::runtime_error("Max connections limit reached");

			std::cout << "[SERVER]: A new connection has been made" << std::endl;

			// Adds the connections as a new client
			this->clients.push_back(Client(connectionFd));

			for (size_t i = 1; i <= clients.size(); i++) // Saves the new connection
			{
				if (this->backlogFds[i].fd != -1)
					continue ;

				this->backlogFds[i].fd = connectionFd;
				this->backlogFds[i].events = POLLIN;
				break ;
			}
		}

		this->handleClientCommunications();
	}
}

void Server::handleClientCommunications()
{
	for (size_t i = 1; i <= clients.size(); i++)
	{
		if (this->backlogFds[i].fd == -1)
			continue ;

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
					continue ;
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
void handleCommand(Client caller, std::string command, std::string body);

void Server::handleClientInput(Client caller, std::string message)
{
	(void) caller;
	size_t spaceSeparator = message.find(' ');
	std::string command = (spaceSeparator == std::string::npos) ? message : message.substr(0, spaceSeparator);
	std::string body = (spaceSeparator == std::string::npos) ? IRC_ENDLINE : message.substr(spaceSeparator + 1);

	size_t endlinePosition = body.find(IRC_ENDLINE);
	if (endlinePosition != std::string::npos) // If the message does not end with '\r\n' should be ignored, but for now we accept it. TODO: change this
		body = body.substr(0, endlinePosition);

	handleCommand(caller, command, body);
}
