#include "../server/Server.hpp"

void Server::privMessage(std::string buff_rx, int fd)
{
	for (std::string::size_type i = 0; i < buff_rx.length(); ++i)
		buff_rx[i] = std::tolower(buff_rx[i]);

	std::string toChannel;
	std::string sendMessage;
	std::string nickname;

	// Find the sender's nickname and the message content
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

	// Send the message to other clients in the same channel
	for (it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getChannel() == toChannel)
		{
			std::cout << "sending msg: " << sendMessage.c_str() << std::endl;
			int retValue = send(it->getSocketFd(), sendMessage.c_str(), sendMessage.size(), 0);
			if (retValue == -1)
			{
				// Handle the send error here
				// Example: perror("send");
			}
		}
	}
}
