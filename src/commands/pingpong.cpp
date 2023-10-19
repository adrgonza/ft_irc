#include "../Server/Server.hpp"

void Server::pongCheck(const std::string &body, const Client &user)
{
	(void)body;
	(void)user;
}

void Server::pingCheck(const std::string &body, const Client &user)
{
	std::cout << "sending ping" << std::endl;
	user.sendMessage(PONG_CMD(body));
}
