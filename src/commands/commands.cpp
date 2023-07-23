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
std::string getWord(const std::string &str, int wordNumber)
{
	if (str.empty())
		return "";

	std::string::size_type startPos = 0;
	std::string::size_type endPos = 0;
	int currentWord = 0;

	while (currentWord < wordNumber)
	{
		startPos = str.find_first_not_of(' ', endPos);
		if (startPos == std::string::npos)
			return "";
		endPos = str.find(' ', startPos);
		if (endPos == std::string::npos)
			endPos = str.length();
		currentWord++;
	}

	return str.substr(startPos, endPos - startPos);
}

void Server::doIrcCommand(std::string buffer, int fd)
{
	std::size_t newlinePos = buffer.find('\n');
	buffer = buffer.substr(0, newlinePos);
	if (!buffer.empty() && buffer.back() == '\r')
		buffer.erase(buffer.size() - 1);

	std::string command = getCommand(buffer);
	std::cout << "Executing command: " << command << "." << std::endl;

	std::vector<Client>::iterator it = findClientByFd(fd);
	if (it != clients.end())
	{
		if (command == "JOIN")
			handleJoin(getWord(buffer, 2), it->getNickname(), fd);
		else if (command == "NICK")
			changeNickName(getWord(buffer, 2), it->getNickname());
		else if (command == "INVITE")
			inviteNick(it->getNickname(), getWord(buffer, 2), getWord(buffer, 3));
		else if (command == "LIST")
			listChannels(it->getNickname(), fd);
		else if (command == "PART")
			partChannel(it->getNickname(), getWord(buffer, 2), fd);
		// else if (command == "MSG")
			//
		// else if (command == "AWAY")
		// else if (command == "ME")
		// else if (command == "PRIVMSG")
		// /query nickname message
		// else if (command == "QUERY")
		// else if (command == "TOPIC")
		// else if (command == "WHOIS")
	}
}
