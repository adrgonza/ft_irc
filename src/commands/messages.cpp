#include "../Server/Server.hpp"

void Server::sayMsg(std::string body, Client &user)
{
	if (user.getChannel() == "")
		return;
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
	Channel *channelObj = getChannelByName(toChannel);
	if (channelObj != NULL && !channelObj->getTopic().empty())
		sendMessage = "PRIVMSG " + toChannel + ":" + channelObj->getTopic() + " :" + body + " " + nickname + "\r\n";
	else
		sendMessage = "PRIVMSG " + toChannel + " :" + body + " " + nickname + "\r\n";

	std::vector<Client>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->getChannel() == toChannel)
		{
			int retValue = send(it->getFd(), sendMessage, sendMessage.size(), 0);
			if (retValue == -1)
			{
				std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
			}
		}
	}
}

void Server::privMessage(std::string body, Client user)
{
	std::size_t newlinePos = body.find('\n');
	body = body.substr(0, newlinePos);

	if (!body.empty() && body.back() == '\r')
		body.erase(body.size() - 1);

	std::string target = getWord(body, 1);
	std::size_t spacePos = body.find(' ') + 1;
	if (spacePos != std::string::npos)
		body = body.substr(spacePos);
	if (!body.empty() && body[0] != ':')
		body = ":" + body;

	std::string lowerTarget = target;
	for (std::string::size_type i = 0; i < target.length(); i++)
		lowerTarget[i] = std::tolower(target[i]);

	if (!channelExists(lowerTarget) && !userExists(lowerTarget))
		user.sendMessage(ERR_NOSUCHNICK(user.getNickname(), target));

	std::string sendMessage;
	if (channelExists(lowerTarget))
	{
		if (!target.empty() && target[0] != '#')
			target = "#" + target;
		std::vector<Client>::iterator it;
		for (it = _clients.begin(); it != _clients.end(); ++it)
		{
			if (it->getChannel() == target && it->getNickname() != user.getNickname())
				it->sendMessage(PRIVMSG_CMD(user.getNickname(), target, body));
		}
	}
	else
	{
		int targetSocket = getClientSocketFdByNickname(lowerTarget);
		if (targetSocket == -1)
			return;
		Client *receiver = findClientByNickname(lowerTarget);
		receiver->sendMessage(PRIVMSG_RECEIVER_CMD(user.getNickname(), target, body));
	}
}