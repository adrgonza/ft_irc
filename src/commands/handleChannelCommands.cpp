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
		channelListMsg = "There are no channels\r\n";
	else
	{
		channelListMsg = ":" + user + "!user@host 321 * Channel :Users Name\r\n";
		for (std::map<std::string, std::vector<std::string> >::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			std::string channelName = it->first;
			channelListMsg += ":" + user + "!user@host 322 * " + channelName + " " + std::to_string(it->second.size()) + " :";
			for (size_t i = 0; i < it->second.size(); ++i)
			{
				channelListMsg += it->second[i];
				if (i < it->second.size() - 1)
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
void Server::partChannel(std::string user, std::string channel, int clientFd)
{
	clientFd += 0;
	std::string leavingChannelMsg = ":" + user + "!user@host PART #" + channel + "\r\n";
	channel = "#" + channel;
	std::map<std::string, std::vector<std::string> >::iterator channelIt = channels.find(channel);
	if (channelIt != channels.end())
	{
		std::vector<std::string> clientsInChannel = channelIt->second;
		for (size_t i = 0; i < clientsInChannel.size(); ++i)
		{
			const std::string &nickname = clientsInChannel[i];
			int clientSocketFd = getClientSocketFdByNickname(nickname);
			std::vector<Client>::iterator it = findClientByFd(clientSocketFd);
			it->changeChannel("test");
			if (clientSocketFd != -1)
			{
				int retValue = send(clientSocketFd, leavingChannelMsg.c_str(), leavingChannelMsg.size(), 0);
				if (retValue == -1)
					std::cerr << "[SERVER-error]: send failed for " << nickname << ": " << errno << strerror(errno) << std::endl;
			}

		}
		clientsInChannel.erase(std::remove(clientsInChannel.begin(), clientsInChannel.end(), user), clientsInChannel.end());
	}
}

void Server::handleJoin(std::string channel, std::string user, int clientFd)
{
	if (channels.find(channel) != channels.end())
	{
		channels[channel].push_back(user);
		std::cout << "User " << user << " joined channel " << channel << std::endl;
	}
	else
	{
		channels[channel] = std::vector<std::string>(1, user);
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
	std::string sendMessage = "JOIN " + channel + "\r\n";
	int retValue = send(clientFd, sendMessage.c_str(), sendMessage.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
}
