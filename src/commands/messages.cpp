#include "../Server/Server.hpp"
#include <iostream>
#include <string>

void Server::sayMsg(std::string &body, const Client &user)
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
	if (!body.empty() && body[body.size() - 1] == '\r')
		body.erase(body.size() - 1);

	nickname = user.getNickname();
	toChannel = user.getChannel();
	Channel *channelObj = getChannelByName(toChannel);
	if (channelObj != NULL && !channelObj->getTopic().empty())
		sendMessage = "PRIVMSG " + toChannel + ":" + channelObj->getTopic() + " :" + body + " " + nickname + "\r\n";
	else
		sendMessage = "PRIVMSG " + toChannel + " :" + body + " " + nickname + "\r\n";

	std::vector<Client *>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it)->getChannel() == toChannel)
			user.sendMessage(sendMessage);
	}
}

void Server::privMessage(std::string &body, const Client &user)
{
	std::size_t newlinePos = body.find('\n');
	body = body.substr(0, newlinePos);

	if (!body.empty() && body[body.size() - 1] == '\r')
		body.erase(body.size() - 1);

	std::string target = getWord(body, 1);
	std::size_t spacePos = body.find(' ') + 1;
	if (spacePos != std::string::npos)
		body = body.substr(spacePos);

	// std::string lowerTarget = target;
	// for (std::string::size_type i = 0; i < target.length(); i++)
	// 	lowerTarget[i] = std::tolower(target[i]);

	if (!channelExists(target) && !userExists(target))
		user.sendMessage(ERR_NOSUCHNICK(user.getNickname(), target));

	std::string sendMessage;
	if (channelExists(target))
	{
		Channel *chanObj = getChannelByName(target);
		if (!chanObj->getAcceptExternalMsgs() && !chanObj->hasParticipant(user))
		{
			user.sendMessage(ERR_NOTONCHANNEL(user.getNickname(), target));
			return;
		}

		if (chanObj->isAdminOnly())
		{
			if (chanObj->isOperator(user) == false)
			{
				user.sendMessage(ERR_CHANOPRIVSNEEDED(user.getNickname(), target));
				return;
			}
		}

		if (!target.empty() && target[0] != '#')
			target = "#" + target;

		for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
			if ((*it)->getChannel() == target && (*it)->getNickname() != user.getNickname())
				(*it)->sendMessage(PRIVMSG_CMD(user.getNickname(), target, body));
		}
	}
	else
	{
		int targetSocket = getClientSocketFdByNickname(target);
		if (targetSocket == -1)
			return;
		Client *receiver = findClientByNickname(target);
		receiver->sendMessage(PRIVMSG_RECEIVER_CMD(user.getNickname(), user.getNickname(), target, body));
	}
}
