#include "../server/Server.hpp"

std::string commands[4] = {"JOIN", "INVITE", "LIST", "ME"};

void welcomeClient()
{
}

std::string getCommand(std::string buffer)
{
	std::string command = "";
	int i = -1;
	while (buffer[++i] && buffer[i] != ' ')
		command += buffer[i];
	return command;
}

bool isIrcCommand(std::string buffer)
{
	std::string command = getCommand(buffer);
	bool possibleCommand = false;
	for (int i = 0; i < 4; i++)
	{
		if (command == commands[i])
		{
			possibleCommand = true;
			break;
		}
	}
	return (possibleCommand);
}

void Server::doIrcCommand(std::string buffer, int fd)
{
	std::string command = getCommand(buffer);
	std::cout << " Executing command: " << command << std::endl;
	if (command == "JOIN")
	{
		std::string channel;
		std::size_t spacePos = buffer.find(' ');
		if (spacePos != std::string::npos)
		{
			channel = buffer.substr(spacePos + 1);
			std::size_t userPos = channel.find(' ');
			if (userPos != std::string::npos)
				channel = channel.substr(0, userPos);
		}
		for (size_t k = 0; k < clients.size(); k++)
		{
			if (clients[k].getSocketFd() == fd)
			{
				handleJoin(channel, clients[k].getNickname());
				break;
			}
		}
	}
}
