#include "../server/Server.hpp"

// Dos usuarios con el mismo nick?
void Server::changeNickName(std::string newNick, std::string oldNick)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getNickname() == oldNick)
		{
			it->changeNickname(newNick);
			std::string sendMessage = ":" + oldNick + "!user@host NICK " + newNick + "\r\n";
			int retValue = send(it->getSocketFd(), sendMessage.c_str(), sendMessage.size(), 0);
			if (retValue == -1)
				std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;

			break;
		}
	}
}
