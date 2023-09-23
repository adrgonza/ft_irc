#include "../Server/Server.hpp"

// /list [#channel] [-MIN #] [-MAX #]
// Lists currently available channels. You can also tell mIRC to show only channels with a minimum and a maximum number of people. If you specify a #channel then mIRC will only list information for that channel. If you specify wildcards, eg. *mirc* then mIRC will list all channels that contain the word mirc in them.
// example:        /list
// example:        /list -min 5 -max 20
// example:        /list #mirc
// example:        /list *mirc*
// :server.example.com 321 * Channel :Users Name
// :server.example.com 322 * #channelName numUsers :user1 user2 user3 ...
// ...
// :server.example.com 323 * :End of channel list

void Server::listChannels(std::string body, Client &user)
{
	body = "";
	std::string channelListMsg = "Channel List:\r\n";

	if (channels.empty())
		channelListMsg = "There are no channels\r\n";
	else
	{
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
	}
	int retValue = send(user.getFd(), channelListMsg.c_str(), channelListMsg.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
}

// :leavingUser!user@host PART #channelName
// If it is the last user in channel, should it erase the channel to?
// sudo for one user in channel? as if to kick somebody?
void Server::partChannel(std::string body, Client &user)
{
	std::string channel = getWord(body, 2);
	if (!channel.empty() && channel[0] != '#')
		channel = "#" + channel;
	std::string leavingChannelMsg = ":" + user.getNickname() + "!user@host PART " + channel + "\r\n";
	if (user.getChannel() != channel)
	{
		// std::string errorMessage = ":" + network + " 442 " + user + " " + user + channel + " :You are not in that channel" + "\r\n";
		std::string errorMessage = ": 442 " + user.getNickname() + " " + user.getNickname() + channel + " :You are not in that channel" + "\r\n";
		int retValue = send(user.getFd(), errorMessage.c_str(), errorMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		return;
	}
	std::map<std::string, Channel>::iterator channelIt = channels.find(channel);
	Channel *channelObj = getChannelByName(channel);
	if (channelObj)
	{
		std::vector<Client> clientsInChannel = channelIt->second.getParticipants();
		channelObj->removeParticipant(user);
		user.changeChannel("");
		for (size_t i = 0; i < clientsInChannel.size(); ++i)
		{
			std::string nickname = clientsInChannel[i].getNickname();
			int clientSocketFd = clientsInChannel[i].getFd();
			if (clientSocketFd != -1)
			{
				int retValue = send(clientSocketFd, leavingChannelMsg.c_str(), leavingChannelMsg.size(), 0);
				if (retValue == -1)
					std::cerr << "[SERVER-error]: send failed for " << nickname << ": " << errno << strerror(errno) << std::endl;
			}
		}
	}
}

// When the last user of a channels parts from it, should the channel be deleted ?
void Server::handleJoin(std::string body, Client &user)
{
	// check body
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

// void Server::topicChannel(std::string channel, std::string newTopic, int clientFd,)
void Server::topicChannel(std::string body, Client &user)
{
	std::string network = "";
	std::string channel = getWord(body, 1);
	std::string newTopic = getWord(body, 2);
	channel = "#" + channel;
	if (user.getChannel() != channel)
	{
		std::string sendMessage = ":" + network + " 442 " + user.getNickname() + "#" + channel + ":You're not on that channel" + "\r\n";
		int retValue = send(user.getFd(), sendMessage.c_str(), sendMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		return;
	}
	if (newTopic.empty())
	{
		// Always tries to set topic and not returns the current topic
		std::string sendMessage = "TOPIC " + channel + "\r\n";
		int retValue = send(user.getFd(), sendMessage.c_str(), sendMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
	}
	else
	{
		Channel *channelObj = getChannelByName(channel);
		if (channelObj != NULL)
			channelObj->setTopic(newTopic);
		std::string sendMessage = "TOPIC " + channel + " :" + newTopic + "\r\n";
		int retValue = send(user.getFd(), sendMessage.c_str(), sendMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
	}
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
	std::string invitingUser = user.getNickname();
	if (!userExists(targetUser))
	{
		std::string errorMessage = ":" + network + " 442 " + invitingUser + " " + invitingUser + "#" + targetUser + " :User doesn't exist" + "\r\n";
		int retValue = send(user.getFd(), errorMessage.c_str(), errorMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		return;
	}
	std::string lookChannel = "#" + channel;
	if (!channelExists(lookChannel))
	{
		std::string errorMessage = ":" + network + " 442 " + invitingUser + " " + invitingUser + "#" + channel + " :Channel doesn't exist" + "\r\n";
		int retValue = send(user.getFd(), errorMessage.c_str(), errorMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		return;
	}
	if (user.getChannel() != lookChannel)
	{
		std::string errorMessage = ":" + network + " 442 " + invitingUser + " " + invitingUser + "#" + channel + " :You are not in the channel" + "\r\n";
		int retValue = send(user.getFd(), errorMessage.c_str(), errorMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		return;
	}

	int clientFd = 0;
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->getNickname() == targetUser)
		{
			if (it->getChannel() == lookChannel)
			{
				std::string errorMessage = ":" + network + " 442 " + invitingUser + " " + invitingUser + "#" + targetUser + " :Is already in the channel" + "\r\n";
				int retValue = send(user.getFd(), errorMessage.c_str(), errorMessage.size(), 0);
				if (retValue == -1)
					std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
				return;
			}
			clientFd = it->getFd();
			break;
		}
	}
	std::string inviteMessage = ":" + invitingUser + "!user@host INVITE " + targetUser + " :" + channel + "\r\n";
	int retValue = send(clientFd, inviteMessage.c_str(), inviteMessage.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
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
		// std::string part = "PART " + channel;
		// partChannel(part, *tarUser);
		chan->removeParticipant(*tarUser);
		tarUser->changeChannel("");
		tarUser->sendMessage(KICK_CMD, nick.c_str(), channel.c_str(), targetUser.c_str());
	}
	else
		user.sendMessage(ERR_CHANOPRIVSNEEDED, nick.c_str(), channel.c_str());
}