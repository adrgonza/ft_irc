#include "../Server/Server.hpp"

void Server::pongCheck(std::string body, Client &user)
{
	(void)body;
	(void)user;
}

void Server::pingCheck(std::string body, Client &user)
{
	body = "" + body;
	std::cout << "sending ping" << std::endl;
	std::string pongMessage = "PONG :" + body + "\r\n";
	user.sendMessage(pongMessage);
}
