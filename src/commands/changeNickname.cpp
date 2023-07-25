#include <Server.hpp>

// Dos usuarios con el mismo nick?
void Server::changeNickName(std::string newNick, std::string oldNick, int clientFd)
{
	if (newNick == ":")
	{
		sendErrorMsgToClient("NICK <nickname> expected.", 442, oldNick, clientFd, "");
		return;
	}
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getNickname() == oldNick)
		{
			it->changeNickname(newNick);
			std::string sendMessage = ":" + oldNick + "!user@host NICK " + newNick + "\r\n";
			sendMsgToClient(sendMessage, it->getSocketFd());
			break;
		}
	}
}
