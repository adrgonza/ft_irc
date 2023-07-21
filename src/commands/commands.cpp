#include "../server/Server.hpp"

std::string commands[4] = {"JOIN", "INVITE", "LIST", "ME"};

void welcomeClient()
{
}

std::string getCommand(std::string buffer) {
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

void doIrcCommand(std::string buffer)
{
	std::string command = getCommand(buffer);
	std::cout << " Executing command: " << command << std::endl;
}
