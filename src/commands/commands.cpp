#include "../server/Server.hpp"

std::string commands[6] = {"JOIN", "INVITE", "LIST", "ME", "NICK", "PART"};

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
	for (int i = 0; i < 6; i++)
	{
		if (command == commands[i])
		{
			possibleCommand = true;
			break;
		}
	}
	return (possibleCommand);
}

std::string getFirstWord(std::string str)
{
	std::string res;
	std::size_t spacePos = str.find(' ');
	if (spacePos != std::string::npos)
	{
		res = str.substr(spacePos + 1);
		std::size_t userPos = res.find(' ');
		if (userPos != std::string::npos)
			res = res.substr(0, userPos);
	}
	return res;
}

void Server::doIrcCommand(std::string buffer, int fd)
{
	std::size_t newlinePos = buffer.find('\n');
	buffer = buffer.substr(0, newlinePos);
	if (!buffer.empty() && buffer.back() == '\r')
		buffer.erase(buffer.size() - 1);
	std::string command = getCommand(buffer);
	std::cout << " Executing command: " << command << std::endl;
	std::string firstWord = getFirstWord(buffer);
	// Creo que seria mejor pasarle la clase del cliente que hacerlo con estos fors feos,
	// minimamente hacer una funcion para que no se repitan

	// crear fucnion de iterator the qevlueva el objeto del cliente qque uiero dependiendo el fd o nickname

	if (command == "JOIN")
	{
		for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			if (it->getSocketFd() == fd)
			{
				handleJoin(firstWord, it->getNickname(), fd);
				break;
			}
		}
	}
	else if (command == "NICK")
	{
		for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			if (it->getSocketFd() == fd)
			{
				changeNickName(firstWord, it->getNickname());
				break;
			}
		}
	}
	else if (command == "INVITE")
	{
		std::string secondWord = "";
		inviteNick(firstWord, secondWord);
	}
	else if (command == "LIST")
		listChannels();
	else if (command == "PART")
		partChannel(firstWord);
}
