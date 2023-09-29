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
			int retValue = send(it->getFd(), sendMessage.c_str(), sendMessage.size(), 0);
			if (retValue == -1)
			{
				std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
			}
		}
	}
}

// Check body, if no text, ERR_NOTEXTTOSEND
// Check body, if no target, ERR_NORECIPIENT
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
		user.sendMessage(ERR_NOSUCHNICK, user.getNickname().c_str(), target.c_str());

	std::string sendMessage;
	if (channelExists(lowerTarget))
	{
		if (!target.empty() && target[0] != '#')
			target = "#" + target;
		std::vector<Client>::iterator it;
		for (it = _clients.begin(); it != _clients.end(); ++it)
		{
			std::string sendMessage = ":" + user.getNickname() + " " + PRIVMSG_CMD;
			if (it->getChannel() == target && it->getNickname() != user.getNickname())
				it->sendMessage(sendMessage, target.c_str(), body.c_str());
		}
	}
	else
	{
		int targetSocket = getClientSocketFdByNickname(lowerTarget);
		std::cout << "socket: " << targetSocket << std::endl;
		if (targetSocket == -1) // target user not in server
			return;
		// user.sendMessage(PRIVMSG_CMD, target.c_str(), body.c_str());

		Client *receiver = findClientByNickname(lowerTarget);
		receiver->sendMessage(PRIVMSG_RECEIVER_CMD, user.getNickname().c_str(), target.c_str(), body.c_str());
	}
}