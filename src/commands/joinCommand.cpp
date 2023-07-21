#include "../server/Server.hpp"

void Server::handleJoin(std::string channel, std::string user)
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
}
