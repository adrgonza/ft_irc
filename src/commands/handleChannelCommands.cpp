#include "../server/Server.hpp"

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
		std::string errorMessage = ": 442 " + user + " :There are no channels to list\r\n";
		int retValue = send(clientFd, errorMessage.c_str(), errorMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
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
	int retValue = send(clientFd, channelListMsg.c_str(), channelListMsg.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
}

// :leavingUser!user@host PART #channelName
// If it is the last user in channel, should it erase the channel too?
// sudo for one user in channel? as if to kick somebody?
void Server::partChannel(std::string user, std::string channelName, int clientFd)
{
	channelName = "#" + channelName;

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
		std::string errorMessage = ":" + network + " 442 " + user + " " + user + channelName + " :You are not in that channel" + "\r\n";
		int retValue = send(clientFd, errorMessage.c_str(), errorMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
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
			{
				int retValue = send(clientSocketFd, leavingChannelMsg.c_str(), leavingChannelMsg.size(), 0);
				if (retValue == -1)
					std::cerr << "[SERVER-error]: send failed for " << nickname << ": " << errno << strerror(errno) << std::endl;
			}
		}
		channel.removeParticipant(user);
		channel.removeOperator(user); // May do nothing if the user is not operator. Intended behaviour.
		// If channel size == 0; remove it
		if (channel.getParticipants().size() == 0)
			std::cout << "CHANNEL ERASED :: "<< channels.erase(channelName) << std::endl;
		std::vector<Client>::iterator it = findClientByFd(clientFd);
		if (it->getChannel() == channelName)
			it->changeChannel("test");
	}
}

// When the last user of a channels parts from it, should the channel be deleted ?
void Server::handleJoin(std::string channel, std::string user, int clientFd)
{
	if (channels.find(channel) != channels.end())
	{
		channels[channel].addParticipant(user);
		std::cout << "User " << user << " joined channel " << channel << std::endl;
	}
	else
	{
		// When a user creates a channel, should it be the admin ?
		Channel newChannel;
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
	int retValue = send(clientFd, sendMessage.c_str(), sendMessage.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
}

void Server::topicChannel(std::string channel, int clientFd, std::string newTopic)
{
	std::vector<Client>::iterator it = findClientByFd(clientFd);
	channel = "#" + channel;
	if (it->getChannel() != channel)
	{
		std::string sendMessage = ":" + network + " 442 " + it->getNickname() + "#" + channel + ":You're not on that channel" + "\r\n";
		int retValue = send(clientFd, sendMessage.c_str(), sendMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		return;
	}
	if (newTopic.empty())
	{
		// Always tries to set topic and not returns the current topic
		std::string sendMessage = "TOPIC " + channel + "\r\n";
		int retValue = send(clientFd, sendMessage.c_str(), sendMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
	}
	else
	{
		Channel *channelObj = getChannelByName(channel);
		if (channelObj != NULL)
			channelObj->setTopic(newTopic);
		std::string sendMessage = "TOPIC " + channel + " :" + newTopic + "\r\n";
		int retValue = send(clientFd, sendMessage.c_str(), sendMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
	}
}

void Server::getNamesInChannel(std::string channel, int clientFd)
{
	std::vector<Client>::iterator it = findClientByFd(clientFd);
	channel = "#" + channel;
	if (it->getChannel() != channel)
	{
		std::string errorMessage = ":" + network + " 442 " + it->getNickname() + " " + it->getNickname() + channel + " :You are not in the channel" + "\r\n";
		int retValue = send(clientFd, errorMessage.c_str(), errorMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		return;
	}
	std::string startOfNamesMessage = ":" + network + " 353 " + it->getNickname() + " = " + channel + " :";
	int retValue = send(clientFd, startOfNamesMessage.c_str(), startOfNamesMessage.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
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
		retValue = send(clientFd, namesInChannelMessage.c_str(), namesInChannelMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
	}
	std::string endOfNamesMessage = ":" + network + " 366 " + it->getNickname() + " " + channel + " :End of NAMES list\r\n";
	retValue = send(clientFd, endOfNamesMessage.c_str(), endOfNamesMessage.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
}

void Server::kickUser(std::string buffer, int clientFd)
{
	std::string channelName = getWord(buffer, 2);
	std::string userToKick = getWord(buffer, 3);
	std::vector<Client>::iterator it = findClientByFd(clientFd);

	Channel* channelObj = getChannelByName(channelName);
	if (channelObj == NULL)
	{
		std::cout << "Channel not found" << std::endl;
		return; // Error. Not a valid channel
	}
	// Check caller is operator
	if (!channelObj->hasOperator(it->getNickname()))
	{
		std::cout << "Not operator" << std::endl;
		return; // Error. It not operator
	}

	// Check if user to kick is in channel
	if (!channelObj->hasUser(userToKick))
	{
		std::cout << "User not in channel" << std::endl;
		return; // Error. User not in channel.
	}

	std::string sendMsg = "Kick " + channelName + " " + userToKick + " \r\n";
	int retValue = send(clientFd, sendMsg.c_str(), sendMsg.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;

	clientFd = getClientSocketFdByNickname(userToKick);
	retValue = send(clientFd, sendMsg.c_str(), sendMsg.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
	channelObj->removeParticipant(userToKick);
	Client* clientObj = findClientByNickname(userToKick);
	clientObj->changeChannel("test");
}
