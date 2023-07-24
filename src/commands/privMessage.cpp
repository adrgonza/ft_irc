#include <Server.hpp>

void Server::privMessage(std::string buff_rx, int fd)
{
	for (std::string::size_type i = 0; i < buff_rx.length(); ++i)
		buff_rx[i] = std::tolower(buff_rx[i]);

	std::string toChannel;
	std::string sendMessage;
	std::string nickname;

	std::size_t newlinePos = buff_rx.find('\n');
	buff_rx = buff_rx.substr(0, newlinePos);
	if (!buff_rx.empty() && buff_rx.back() == '\r')
		buff_rx.erase(buff_rx.size() - 1);

	bool clientFound = false;
	std::vector<Client>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getSocketFd() == fd)
		{
			nickname = it->getNickname();
			toChannel = it->getChannel();
			Channel* channelObj = getChannelByName(toChannel);
			if (channelObj != NULL && !channelObj->getTopic().empty())
				sendMessage = "PRIVMSG " + toChannel + ":" + channelObj->getTopic() + " :" + buff_rx + " " + nickname + "\r\n";
			else
				sendMessage = "PRIVMSG " + toChannel + " :" + buff_rx + " " + nickname + "\r\n";
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
			int retValue = send(it->getSocketFd(), sendMessage.c_str(), sendMessage.size(), 0);
			if (retValue == -1)
			{
				std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
			}
		}
	}
}
