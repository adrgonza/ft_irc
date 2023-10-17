#include "../Server/Server.hpp"

void Server::quitServ(std::string body, Client &user)
{
	if (body == ":")
		body = "";
	std::string nick = user.getNickname();
	user.sendMessage(QUIT_CMD(nick, nick, body));

	int i = 0;
	while (_pollFds[i].fd != user.getFd())
		i++;

	disconnectClient(i);
}
