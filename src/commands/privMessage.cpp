#include "../server/Server.hpp"

void Server::privMessage(std::string buff_rx, int fd)
{
	for (std::string::size_type i = 0; i < buff_rx.length(); ++i)
		buff_rx[i] = std::tolower(buff_rx[i]);
	for (unsigned long j = 1; j <= clients.size(); j++)
	{
		std::string sendMessage;
		bool clientFound = false;
		for (size_t k = 0; k < clients.size(); k++)
		{
			if (clients[k].getSocketFd() == fd)
			{
				const std::string &nickname = clients[k].getNickname();
				std::size_t newlinePos = buff_rx.find('\n');
				buff_rx = buff_rx.substr(0, newlinePos);
				sendMessage = "PRIVMSG test :" + buff_rx + " " + nickname + "\r\n";
				clientFound = true;
				break;
			}
		}
		if (!clientFound)
		{
			cout_msg("Client not found");
			continue;
		}
		int retValue = send(fds[j].fd, sendMessage.c_str(), sendMessage.size(), 0);
		retValue += 0;
		// std::cout << " · (" << i << ") -> (" << j << ") :: (status: " << retValue << ") " << std::endl;
	}
}
