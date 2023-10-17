#include "../Server/Server.hpp"

void Server::listChannels(std::string body, Client &user)
{
	body = "";
	std::string channelListMsg = "Channel List:\r\n";

	if (_channels.empty())
	{
		user.sendMessage(ERR_NOCHANNELS(user.getNickname()));
		return;
	}
	channelListMsg = ":" + user.getNickname() + "!user@host 321 * Channel :Users Name\r\n";
	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		std::string channelName = it->first;

		std::stringstream participantCount;
		participantCount << it->second.getParticipants().size();

		channelListMsg += ":" + user.getNickname() + "!user@host 322 * " + channelName + " " + participantCount.str() + " :";
		std::vector<Client> participants = it->second.getParticipants();
		for (size_t i = 0; i < participants.size(); ++i)
		{
			channelListMsg += participants[i].getNickname();
			if (i < participants.size() - 1)
				channelListMsg += " ";
		}
		channelListMsg += "\r\n";
	}
	channelListMsg += ":" + user.getNickname() + "!user@host 323 * :End of channel list";
	user.sendMessage(channelListMsg);
}

void Server::partChannel(std::string body, Client &user)
{
	std::string channel = getWord(body, 1);
	if ((!channel.empty() && channel[0] != '#') || (!channelExists(channel)))
	{
		user.sendMessage(ERR_NOSUCHCHANNEL(user.getNickname(), channel));
		return;
	}
	Channel *channelObj = getChannelByName(channel);
	if (!channelObj->hasParticipant(user))
	{
		user.sendMessage(ERR_NOTONCHANNEL(user.getNickname(), channel));
		return;
	}
	std::vector<Client> clientsInChannel = channelObj->getParticipants();
	for (size_t i = 0; i < clientsInChannel.size(); ++i)
		clientsInChannel[i].sendMessage(PART_CMD(user.getNickname(), channel));
	channelObj->removeParticipant(user);
	user.changeChannel("");
	if (channelObj->getParticipants().empty())
	{
		std::map<std::string, Channel>::iterator it;
		it = _channels.find(channel);
		if (it != _channels.end())
			_channels.erase(it);
	}
}

void Server::handleJoin(std::string body, Client &user)
{
	if (body == "#")
		return;
	std::string channel = body;
	if (channel == user.getChannel() || channel.empty())
		return;
	bool oper = false;
	if (_channels.find(channel) != _channels.end())
	{
		Channel *targetChannel = getChannelByName(channel);
		if (targetChannel->isBanned(user))
		{
			user.sendMessage(ERR_YOUREBANNEDCREEP(user.getNickname()));
			return;
		}
		_channels[channel].addParticipant(user);
		std::cout << "User " << user.getNickname() << " joined channel " << channel << std::endl;
	}
	else
	{
		Channel newChannel;
		newChannel.addParticipant(user);
		newChannel.addOperator(user);
		newChannel.setName(channel);
		newChannel.setTopic("");
		_channels[channel] = newChannel;
		std::cout << "User " << user.getNickname() << " created and joined channel " << channel << std::endl;
		oper = true;
	}
	user.changeChannel(channel);

	Channel *toChan = getChannelByName(channel);
	std::vector<Client> clientsInChannel = toChan->getParticipants();
	std::string nick = user.getNickname();
	if (oper == true)
	{
		for (std::vector<Client>::iterator it = clientsInChannel.begin(); it != clientsInChannel.end(); ++it)
		{
			it->sendMessage(JOIN_CMD(nick, nick, channel));
			it->sendMessage(RPL_NAMREPLY(nick, "=", channel, ("@" + nick)));
			it->sendMessage(RPL_ENDOFNAMES(nick, channel));
		}
	}
	else
	{
		std::vector<Client> clientOperatorsInChan = toChan->getOperators();
		std::string listNames = "";
		for (std::vector<Client>::iterator it = clientOperatorsInChan.begin(); it != clientOperatorsInChan.end(); ++it)
			listNames += "@" + it->getNickname() + " ";
		for (std::vector<Client>::iterator it = clientsInChannel.begin(); it != clientsInChannel.end(); ++it)
		{
			bool addClient = true;
			for (std::vector<Client>::iterator operatorIt = clientOperatorsInChan.begin(); operatorIt != clientOperatorsInChan.end(); ++operatorIt)
			{
				if (operatorIt->getNickname() == it->getNickname())
				{
					addClient = false;
					break;
				}
			}
			if (addClient)
				listNames += it->getNickname() + " ";
		}

		for (std::vector<Client>::iterator it = clientsInChannel.begin(); it != clientsInChannel.end(); ++it)
		{
			it->sendMessage(JOIN_CMD(nick, nick, channel));
			if (!toChan->getTopic().empty() && toChan->getTopic() != "")
				it->sendMessage(TOPIC_CMD(channel, toChan->getTopic()));
			it->sendMessage(RPL_NAMREPLY(nick, "=", channel, listNames));
			it->sendMessage(RPL_ENDOFNAMES(nick, channel));
		}
	}
}

void Server::topicChannel(std::string body, Client &user)
{
	std::istringstream iss(body);
	std::string channel;
	iss >> channel;
	std::string newTopic;
	std::getline(iss, newTopic);

	if (!channel.empty() && channel[0] != '#')
		channel = "#" + channel;
	if (!channelExists(channel))
	{
		user.sendMessage(ERR_NOSUCHCHANNEL(user.getNickname(), channel));
		return;
	}
	Channel *channelObj = getChannelByName(channel);
	if (newTopic.empty())
	{
		std::string sendMsg = "332 TOPIC: " + channelObj->getTopic() + IRC_ENDLINE;
		sendMsg = "332 " + user.getNickname() + " " + channel + " :" + channelObj->getTopic() + IRC_ENDLINE;
		user.sendMessage(sendMsg);
		return;
	}

	if (!newTopic.empty() && newTopic[0] == ' ')
		newTopic = newTopic.substr(1);
	std::string network = "";
	if (!channelObj->hasParticipant(user))
	{
		user.sendMessage(ERR_NOTONCHANNEL(user.getNickname(), channel));
		return;
	}
	channelObj->setTopic(newTopic);
	user.sendMessage(TOPIC_CMD(channel, newTopic));
}

void Server::getNamesInChannel(std::string body, Client &user)
{
	std::string channel = "#" + body;
	if (user.getChannel() != channel)
	{
		std::string errorMessage = ": 442 " + user.getNickname() + " " + user.getNickname() + channel + " :You are not in the channel" + "\r\n";
		user.sendMessage(errorMessage);
		return;
	}
	std::string startOfNamesMessage = ": 353 " + user.getNickname() + " = " + channel + " :";
	user.sendMessage(startOfNamesMessage);
	std::map<std::string, Channel>::iterator channelIt = _channels.find(channel);
	if (channelIt != _channels.end())
	{
		std::vector<Client> clientsInChannel = channelIt->second.getParticipants();
		std::string namesInChannelMessage;
		std::cout << channelIt->first << " size: " << clientsInChannel.size() << std::endl;
		std::vector<Client>::iterator it;
		for (it = clientsInChannel.begin(); it != clientsInChannel.end(); ++it)
		{
			const Client &client = *it;
			std::cout << "wach " << client.getChannel() << std::endl;
			namesInChannelMessage += client.getNickname() + " ";
		}
		std::cout << "names in channel " << namesInChannelMessage << std::endl;
		user.sendMessage(namesInChannelMessage);
	}
	std::cout << "asdas" << std::endl;
	std::string endOfNamesMessage = ": 366 " + user.getNickname() + " " + channel + " :End of NAMES list\r\n";
	user.sendMessage(endOfNamesMessage);
}

void Server::inviteNick(std::string body, Client &user)
{
	std::string network = "";
	std::string targetUser = getWord(body, 1);
	std::string channel = getWord(body, 2);
	if (!channel.empty() && channel[0] != '#')
		channel = "#" + channel;
	std::string invitingUser = user.getNickname();
	if (!userExists(targetUser))
	{
		user.sendMessage(ERR_NOSUCHNICK(user.getNickname(), targetUser));
		return;
	}
	if (!channelExists(channel))
	{
		user.sendMessage(ERR_NOSUCHCHANNEL(user.getNickname(), channel));
		return;
	}
	Channel *channelObj = getChannelByName(channel);
	if (!channelObj->hasParticipant(user))
	{
		user.sendMessage(ERR_NOTONCHANNEL(user.getNickname(), channel));
		return;
	}
	Client *targetClient = findClientByNickname(targetUser);
	if (channelObj->hasParticipant(*targetClient))
	{
		user.sendMessage(ERR_USERONCHANNEL(user.getNickname(), targetUser, channel));
		return;
	}
	targetClient->sendMessage(INVITE_CMD(user.getNickname(), targetUser, channel));
}
