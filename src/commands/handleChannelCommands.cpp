#include "../Server/Server.hpp"

void Server::listChannels(std::string body, Client &user)
{
	body = "";
	std::string channelListMsg = "Channel List:\r\n";

	if (channels.empty())
	{
		user.sendMessage(ERR_NOCHANNELS, user.getNickname().c_str());
		return;
	}
	channelListMsg = ":" + user.getNickname() + "!user@host 321 * Channel :Users Name\r\n";
	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		std::string channelName = it->first;
		channelListMsg += ":" + user.getNickname() + "!user@host 322 * " + channelName + " " + std::to_string(it->second.getParticipants().size()) + " :";
		std::vector<Client> participants = it->second.getParticipants();
		for (size_t i = 0; i < participants.size(); ++i)
		{
			channelListMsg += participants[i].getNickname();
			if (i < participants.size() - 1)
				channelListMsg += " ";
		}
		channelListMsg += "\r\n";
	}
	channelListMsg += ":" + user.getNickname() + "!user@host 323 * :End of channel list\r\n";

	int retValue = send(user.getFd(), channelListMsg.c_str(), channelListMsg.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
}

// When the last user of a channels parts from it, should the channel be deleted ?
void Server::partChannel(std::string body, Client &user)
{
	// check body
	std::string channel = getWord(body, 1);
	if (!channel.empty() && channel[0] != '#')
		channel = "#" + channel;
	if (!channelExists(channel))
	{
		user.sendMessage(ERR_NOSUCHCHANNEL, user.getNickname().c_str(), channel.c_str());
		return;
	}
	Channel *channelObj = getChannelByName(channel);
	if (!channelObj->hasParticipant(user))
	{
		user.sendMessage(ERR_NOTONCHANNEL, user.getNickname().c_str(), channel.c_str());
		return;
	}
	std::vector<Client> clientsInChannel = channelObj->getParticipants();
	for (size_t i = 0; i < clientsInChannel.size(); ++i)
	{
		clientsInChannel[i].sendMessage(PART_CMD, user.getNickname().c_str(), channel.c_str());
	}
	channelObj->removeParticipant(user);
	user.changeChannel("");
}

void Server::handleJoin(std::string body, Client &user)
{
	// check body
	if (body == "#")
		return;
	std::string channel = body;
	if (channel == user.getChannel())
		return;
	if (channels.find(channel) != channels.end())
	{
		channels[channel].addParticipant(user);
		std::cout << "User " << user.getNickname() << " joined channel " << channel << std::endl;
	}
	else
	{
		// When a user creates a channel, should it be the admin/operator ?
		Channel newChannel;
		newChannel.addParticipant(user);
		newChannel.addOperator(user);
		channels[channel] = newChannel;
		std::cout << "User " << user.getNickname() << " created and joined channel " << channel << std::endl;
	}
	user.changeChannel(channel);

	Channel *toChan = getChannelByName(channel);
	std::vector<Client> clientsInChannel = toChan->getParticipants();
	std::string sendMessage = ":" + user.getNickname() + " " + JOIN_CMD;
	for (std::vector<Client>::iterator it = clientsInChannel.begin(); it != clientsInChannel.end(); ++it)
		it->sendMessage(sendMessage, channel.c_str());
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
		user.sendMessage(ERR_NOSUCHCHANNEL, user.getNickname().c_str(), channel.c_str());
		return;
	}
	Channel *channelObj = getChannelByName(channel);
	if (newTopic.empty())
	{
		std::string sendMsg = "332 TOPIC: " + channelObj->getTopic() + IRC_ENDLINE;
		sendMsg = "332 " + user.getNickname() + " " + channel + " :" + channelObj->getTopic() + IRC_ENDLINE;
		user.sendMessage(sendMsg);
		return ;
	}

    if (!newTopic.empty() && newTopic[0] == ' ')
        newTopic = newTopic.substr(1);
	std::string network = "";
	if (!channelObj->hasParticipant(user))
	{
		user.sendMessage(ERR_NOTONCHANNEL, user.getNickname().c_str(), channel.c_str());
		return;
	}
	channelObj->setTopic(newTopic);
	user.sendMessage(TOPIC_CMD, channel.c_str(), newTopic.c_str());
}

void Server::getNamesInChannel(std::string body, Client &user)
{
	std::string channel = "#" + body;
	if (user.getChannel() != channel)
	{
		// std::string errorMessage = ":" + network + " 442 " + user.getNickname() + " " + user.getNickname() + channel + " :You are not in the channel" + "\r\n";
		std::string errorMessage = ": 442 " + user.getNickname() + " " + user.getNickname() + channel + " :You are not in the channel" + "\r\n";
		int retValue = send(user.getFd(), errorMessage.c_str(), errorMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		return;
	}
	// std::string startOfNamesMessage = ":" + network + " 353 " + it->getNickname() + " = " + channel + " :";
	std::string startOfNamesMessage = ": 353 " + user.getNickname() + " = " + channel + " :";
	int retValue = send(user.getFd(), startOfNamesMessage.c_str(), startOfNamesMessage.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
	std::map<std::string, Channel>::iterator channelIt = channels.find(channel);
	if (channelIt != channels.end())
	{
		std::vector<Client> clientsInChannel = channelIt->second.getParticipants();
		std::string namesInChannelMessage;
		std::cout << channelIt->first << " size: " << clientsInChannel.size() << std::endl;
		std::vector<Client>::iterator it;
		for (it = clientsInChannel.begin(); it != clientsInChannel.end(); ++it)
		{
			const Client &client = *it;
			std::cout << "wach " << client.getChannel() << std::endl;
			// Aca habria que agregar @ or + dependiendo si es operador o no
			// namesInChannelMessage += "@";
			namesInChannelMessage += client.getNickname() + " ";
		}
		std::cout << "names in channel " << namesInChannelMessage << std::endl;
		namesInChannelMessage += "\r\n";
		retValue = send(user.getFd(), namesInChannelMessage.c_str(), namesInChannelMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
	}
	std::cout << "asdas" << std::endl;
	// std::string endOfNamesMessage = ":" + network + " 366 " + it->getNickname() + " " + channel + " :End of NAMES list\r\n";
	std::string endOfNamesMessage = ": 366 " + user.getNickname() + " " + channel + " :End of NAMES list\r\n";
	retValue = send(user.getFd(), endOfNamesMessage.c_str(), endOfNamesMessage.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
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
		user.sendMessage(ERR_NOSUCHNICK, user.getNickname().c_str(), targetUser.c_str());
		return;
	}
	if (!channelExists(channel))
	{
		user.sendMessage(ERR_NOSUCHCHANNEL, user.getNickname().c_str(), channel.c_str());
		return;
	}
	Channel *channelObj = getChannelByName(channel);
	if (!channelObj->hasParticipant(user))
	{
		user.sendMessage(ERR_NOTONCHANNEL, user.getNickname().c_str(), channel.c_str());
		return;
	}
	Client* targetClient = findClientByNickname(targetUser);
	if (channelObj->hasParticipant(*targetClient))
	{
		user.sendMessage(ERR_USERONCHANNEL, user.getNickname().c_str(), targetUser.c_str(), channel.c_str());
		return ;
	}
	targetClient->sendMessage(INVITE_CMD, user.getNickname().c_str(), targetUser.c_str(), channel.c_str());
}

void Server::kickUser(std::string body, Client &user)
{
	std::string channel = getWord(body, 1);
	std::string targetUser = getWord(body, 2);

	if (!channelExists(channel))
	{
		std::cout << "Channel does not exist" << std::endl;
		return;
	}
	if (!userExists(targetUser))
	{
		std::cout << "User does not exist" << std::endl;
		return;
	}
	Channel *chan = getChannelByName(channel);
	std::string nick = user.getNickname();
	if (chan->isOperator(user))
	{	
		user.sendMessage(KICK_CMD, nick.c_str(), channel.c_str(), targetUser.c_str());
		Client *tarUser = findClientByNickname(targetUser);
		chan->removeParticipant(*tarUser);
		tarUser->changeChannel("");
		tarUser->sendMessage(KICK_CMD, nick.c_str(), channel.c_str(), targetUser.c_str());
	}
	else
		user.sendMessage(ERR_CHANOPRIVSNEEDED, nick.c_str(), channel.c_str());
}