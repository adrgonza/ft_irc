#include <Server.hpp>

void Server::sayMsg(std::string body, Client &user)
{
	if (user.getChannel() == "")
		return ;
	for (std::string::size_type i = 0; i < body.length(); ++i)
		body[i] = std::tolower(body[i]);

	std::string toChannel;
	std::string sendMessage;
	std::string nickname;

	std::size_t newlinePos = body.find('\n');
	body = body.substr(0, newlinePos);
	if (!body.empty() && body.back() == '\r')
		body.erase(body.size() - 1);

	nickname = user.getNickname();
	toChannel = user.getChannel();
	Channel* channelObj = getChannelByName(toChannel);
	if (channelObj != NULL && !channelObj->getTopic().empty())
		sendMessage = "PRIVMSG " + toChannel + ":" + channelObj->getTopic() + " :" + body + " " + nickname + "\r\n";
	else
		sendMessage = "PRIVMSG " + toChannel + " :" + body + " " + nickname + "\r\n";

	std::vector<Client>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getChannel() == toChannel)
		{
			int retValue = send(it->getFd(), sendMessage.c_str(), sendMessage.size(), 0);
			if (retValue == -1)
			{
				std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
			}
		}
	}
}

void Server::privMessage(std::string body, Client user)
{
	for (std::string::size_type i = 0; i < body.length(); ++i)
		body[i] = std::tolower(body[i]);

	std::string toChannel;
	std::string sendMessage;
	std::string nickname;

	std::size_t newlinePos = body.find('\n');
	body = body.substr(0, newlinePos);
	if (!body.empty() && body.back() == '\r')
		body.erase(body.size() - 1);

	bool clientFound = false;
	std::vector<Client>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getFd() == user.getFd())
		{
			nickname = it->getNickname();
			toChannel = it->getChannel();
			Channel* channelObj = getChannelByName(toChannel);
			if (channelObj != NULL && !channelObj->getTopic().empty())
				sendMessage = "PRIVMSG " + toChannel + ":" + channelObj->getTopic() + " :" + body + " " + nickname + "\r\n";
			else
				sendMessage = "PRIVMSG " + toChannel + " :" + body + " " + nickname + "\r\n";
			clientFound = true;
			break;
		}
	}

	if (!clientFound)
	{
        std::cout << "Client not found" << std::endl;
		return;
	}

	for (it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getChannel() == toChannel)
		{
			int retValue = send(it->getFd(), sendMessage.c_str(), sendMessage.size(), 0);
			if (retValue == -1)
			{
				std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
			}
		}
	}
}
