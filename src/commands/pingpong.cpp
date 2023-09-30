#include "../Server/Server.hpp"

void Server::pongCheck(std::string body, Client &user)
{
	(void)body;
	user.setPing(false);
}

void Server::pingCheck(std::string body, Client &user)
{
	body = "" + body;
	std::cout << "sending ping" << std::endl;
	user.sendMessage(PONG_CMD(body));
}
