#include "../server/Server.hpp"

// /list [#channel] [-MIN #] [-MAX #]
// Lists currently available channels. You can also tell mIRC to show only channels with a minimum and a maximum number of people. If you specify a #channel then mIRC will only list information for that channel. If you specify wildcards, eg. *mirc* then mIRC will list all channels that contain the word mirc in them.
// example:        /list
// example:        /list -min 5 -max 20
// example:        /list #mirc
// example:        /list *mirc*

void Server::listChannels()
{
	if (channels.empty())
		cout_msg("No channels");
	else
	{
		for (std::map<std::string, std::vector<std::string> >::iterator it = channels.begin(); it != channels.end(); ++it)
			cout_msg(it->first);
	}
}

void Server::partChannel(std::string channel)
{
	channel += "";
	// leaves channel
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
