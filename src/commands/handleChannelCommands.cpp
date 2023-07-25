#include <Server.hpp>

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

void Server::listChannels(std::string user, int clientFd)
{
	std::string channelListMsg = "Channel List:\r\n";

	if (channels.empty())
	{
		sendErrorMsgToClient("There are no channels to list", 442, user, clientFd, "");
		return ;
	}
	else
	{
		channelListMsg = ":" + user + "!user@host 321 * Channel :Users Name\r\n";
		for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			std::string channelName = it->first;
			channelListMsg += ":" + user + "!user@host 322 * " + channelName + " " + std::to_string(it->second.getParticipants().size()) + " :";
			std::vector<std::string> participants = it->second.getParticipants();
			for (size_t i = 0; i < participants.size(); ++i)
			{
				channelListMsg += participants[i];
				if (i < participants.size() - 1)
					channelListMsg += " ";
			}
			channelListMsg += "\r\n";
		}
		channelListMsg += ":" + user + "!user@host 323 * :End of channel list\r\n";
	}
	sendMsgToClient(channelListMsg, clientFd);
}

// :leavingUser!user@host PART #channelName
// If it is the last user in channel, should it erase the channel too?
// sudo for one user in channel? as if to kick somebody?
void Server::partChannel(std::string buffer, std::string user, int clientFd)
{
	std::string channelName = getWord(buffer, 2);
	if (channelName == ":" || channelName.empty())
	{
		sendErrorMsgToClient("PART <channel> expected.", 442, user, clientFd, "");
		return;
	}
	if (channelName[0] != '#')
	{
		sendErrorMsgToClient("Channel has to start with #", 442, user, clientFd, channelName);
		return ;
	}
	std::vector<Client>::iterator clientIterator = findClientByFd(clientFd);
	if (clientIterator == clients.end())
		return ; // TODO: Client not found error <??>

	std::string leavingChannelMsg = ":" + user + "!user@host PART " + channelName + "\r\n";

	std::map<std::string, Channel>::iterator channelIterator = channels.find(channelName);
	if (channelIterator == channels.end())
		return; // Todo: Channel does not exist error

	Channel &channel = channelIterator->second;
	bool userInChannel = channel.hasUser(user);
	if (!userInChannel && clientIterator->getChannel() != channelName)
	{
		sendErrorMsgToClient("You are not in that channel", 442, user, clientFd, channelName);
		return;
	}
	if (channelIterator != channels.end())
	{
		std::vector<std::string> clientsInChannel = channel.getParticipants();
		for (size_t i = 0; i < clientsInChannel.size(); ++i)
		{
			std::string nickname = clientsInChannel[i];
			int clientSocketFd = getClientSocketFdByNickname(nickname);
			if (clientSocketFd != -1)
				sendMsgToClient(leavingChannelMsg, clientSocketFd);
		}
		Client client = *(findClientByFd(clientFd)); // Sends them to LIMBO_CHAT if needed
		channel.removeParticipant(&client);
		channel.removeOperator(user); // May do nothing if the user is not operator. Intended behaviour.
		// If channel size == 0; remove it
		if (channel.getParticipants().size() == 0)
			channels.erase(channelName);
	}
}

// When the last user of a channels parts from it, should the channel be deleted ?
void Server::handleJoin(std::string buffer, std::string user, int clientFd)
{
	std::string channel = getWord(buffer, 2);
	if (channel.empty() || channel == "#")
	{
		sendErrorMsgToClient("JOIN <channel> expected, no channel param found", 442, user, clientFd, "");
		return;
	}
	if (channels.find(channel) != channels.end())
	{
		channels[channel].addParticipant(user);
		std::cout << "User " << user << " joined channel " << channel << std::endl;
	}
	else
	{
		// When a user creates a channel, should it be the admin ?
		Channel newChannel = Channel(channel);
		newChannel.addParticipant(user);
		newChannel.addOperator(user);
		channels[channel] = newChannel;
		std::cout << "User " << user << " created and joined channel " << channel << std::endl;
	}
	std::vector<Client>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getSocketFd() == clientFd)
		{
			it->changeChannel(channel);
			break;
		}
	}
	Channel* channelObj = getChannelByName(channel);
	std::string sendMessage;
	if (channelObj != NULL && !channelObj->getTopic().empty())
		sendMessage = "JOIN " + channel + ":" + channelObj->getTopic() + "\r\n";
	else
		sendMessage = "JOIN " + channel + "\r\n";
	sendMsgToClient(sendMessage, clientFd);
}

// void Server::topicChannel(std::string channel, int clientFd, std::string newTopic)
void Server::topicChannel(std::string buffer, std::string user, int clientFd)
{
	std::string channel = getWord(buffer, 2);
	std::string newTopic = getWord(buffer, 3);
	if (channel == ":")
	{
		sendErrorMsgToClient("TOPIC <channel> [<topic>] expected.", 442, user, clientFd, "");
		return;
	}
	if (channel[0] != '#')
	{
		sendErrorMsgToClient("Channel has to start with #", 442, user, clientFd, channel);
		return ;
	}
	std::vector<Client>::iterator it = findClientByFd(clientFd);
	if (it->getChannel() != channel)
	{
		sendErrorMsgToClient("You're not on that channel", 442, user, clientFd, channel);
		return;
	}
	if (newTopic.empty())
	{
		// Always tries to set topic and not returns the current topic
		std::string sendMessage = "TOPIC " + channel + "\r\n";
		sendMsgToClient(sendMessage, clientFd);
	}
	else
	{
		Channel *channelObj = getChannelByName(channel);
		if (channelObj != NULL)
			channelObj->setTopic(newTopic);
		std::string sendMessage = "TOPIC " + channel + ":" + newTopic + "\r\n";
		sendMsgToClient(sendMessage, clientFd);
	}
}

void Server::getNamesInChannel(std::string buffer, int clientFd)
{
	std::vector<Client>::iterator it = findClientByFd(clientFd);
	std::string channel = getWord(buffer, 2);
	if (channel.empty())
	{
		sendErrorMsgToClient("NAMES <channel>{,<channel>} expected.", 442, it->getNickname(), clientFd, "");
		return;
	}
	if (channel[0] != '#')
	{
		sendErrorMsgToClient("Channel has to start with #", 442, it->getNickname(), clientFd, channel);
		return ;
	}
	if (it->getChannel() != channel)
	{
		sendErrorMsgToClient("You're not on that channel", 442, it->getNickname(), clientFd, channel);
		return;
	}
	std::string startOfNamesMessage = ":" + network + " 353 " + it->getNickname() + " = " + channel + " :";
	sendMsgToClient(startOfNamesMessage, clientFd);
	std::map<std::string, Channel>::iterator channelIt = channels.find(channel);
	if (channelIt != channels.end())
	{
		std::vector<std::string> clientsInChannel = channelIt->second.getParticipants();
		std::string namesInChannelMessage;
		for (size_t i = 0; i < clientsInChannel.size(); ++i)
		{
			// Aca habria que agregar @ or + dependiendo si es operador o no
			// namesInChannelMessage += "@";
			namesInChannelMessage += clientsInChannel[i] + " ";
		}
		namesInChannelMessage += "\r\n";
		sendMsgToClient(namesInChannelMessage, clientFd);
	}
	std::string endOfNamesMessage = ":" + network + " 366 " + it->getNickname() + " " + channel + " :End of NAMES list\r\n";
	sendMsgToClient(endOfNamesMessage, clientFd);
}

void Server::kickUser(std::string buffer, int clientFd)
{
	std::string channelName = getWord(buffer, 2);
	std::string userToKick = getWord(buffer, 3);
	std::vector<Client>::iterator it = findClientByFd(clientFd);

	if (channelName == ":" || userToKick == ":")
	{
		// ERR_NEEDMOREPARAMS (461)
		std::string clientNickname = it->getNickname();
		std::string errorMsg = ":" + clientNickname + " 461 : Not enough parameters\r\n";
		int retValue = send(clientFd, errorMsg.c_str(), errorMsg.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		std::cout << "Error code: " << retValue << std::endl << "Error: " << errorMsg << std::endl;
		//"<client> <command> :Not enough parameters"
		return ; // Generalize error sending (clientFd is a must. Some commands require the origin command. Maybe FD and Parameter ??)
	}


	Channel* channelObj = getChannelByName(channelName);
	if (channelObj == NULL)
	{
		sendErrorMsgToClient("Channel not found", 442, it->getNickname(), clientFd, channelName);
		return;
	}
	if (!channelObj->hasOperator(it->getNickname()))
	{
		sendErrorMsgToClient("You are not an operator", 442, it->getNickname(), clientFd, "");
		return;
	}
	if (!channelObj->hasUser(userToKick))
	{
		sendErrorMsgToClient("You are not in the channel", 442, it->getNickname(), clientFd, channelName);
		return;
	}

	// Send caller a confirmation message
	std::string sendMsg = "Kick " + channelName + " " + userToKick + " \r\n";
	sendMsgToClient(sendMsg, clientFd);

	// Send client being kicked an informative message
	Client* clientObj = findClientByNickname(userToKick);
	clientFd = clientObj->getSocketFd();

	// Remove 'userToKick' from the server & send them to LIMBO_CHANNEL if needed
	if (clientObj != NULL)
		channelObj->removeParticipant(clientObj);
	clientFd = getClientSocketFdByNickname(userToKick);
	sendMsgToClient(sendMsg, clientFd);

	channelObj->removeParticipant(clientObj);
}
