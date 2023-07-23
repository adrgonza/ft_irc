#include "../server/Server.hpp"

void Server::respondWithPong(int clientFd)
{
	std::string pongMessage = "PONG :" + network + "\r\n";
	int retValue = send(clientFd, pongMessage.c_str(), pongMessage.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
}
