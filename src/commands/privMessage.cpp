#include <Server.hpp>

void Server::privMessage(std::string buff_rx, int fd)
{
	for (std::string::size_type i = 0; i < buff_rx.length(); ++i)
		buff_rx[i] = std::tolower(buff_rx[i]);

	std::string toChannel;
	std::string message;
	std::string nickname;

	std::size_t newlinePos = buff_rx.find('\n');
	buff_rx = buff_rx.substr(0, newlinePos);
	if (!buff_rx.empty() && buff_rx.back() == '\r')
		buff_rx.erase(buff_rx.size() - 1);

	std::cout << "Called privmsg!" << std::endl;

	bool clientFound = false;
	std::vector<Client>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it)
	{
		std::cout << "loop -> " << it->getSocketFd() << "/" << fd << std::endl;
		if (it->getSocketFd() == fd)
		{
			nickname = it->getNickname();
			toChannel = it->getChannel();
			//Channel* channelObj = getChannelByName(toChannel);
			message = "PRIVMSG " + toChannel + " : " + buff_rx + "\r\n";
			/*if (channelObj != NULL && !channelObj->getTopic().empty())
				sendMessage = "PRIVMSG " + toChannel + ":" + channelObj->getTopic() + " :" + buff_rx + " " + nickname + "\r\n";
			else
				sendMessage = "PRIVMSG " + toChannel + " :" + buff_rx + " " + nickname + "\r\n";*/
			clientFound = true;
			break;
		}
	}

	if (!clientFound)
	{
		cout_msg("Client not found");
		return;
	}

	for (it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getChannel() == toChannel)
		{
			sendMessage(it->getSocketFd(), message);
		}
	}
}
