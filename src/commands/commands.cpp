#include <Server.hpp>

#define NBR_COMMANDS 14

std::string commands[NBR_COMMANDS] = {"JOIN", "INVITE", "LIST", "ME", "NICK", "PART", "PRIVMSG", "TOPIC", "NAMES", "WHO", "PING", "NOTICE", "WHOIS", "ISON"};

void welcomeClient()
{
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

bool isIrcCommand(std::string buffer)
{
	std::string command = getWord(buffer, 1);
	bool possibleCommand = false;
	for (int i = 0; i < NBR_COMMANDS; i++)
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
	std::size_t newlinePos = buffer.find('\n');
	buffer = buffer.substr(0, newlinePos);
	if (!buffer.empty() && buffer.back() == '\r')
		buffer.erase(buffer.size() - 1);

	std::string command = getWord(buffer, 1);
	std::vector<Client>::iterator it = findClientByFd(fd);
	if (it != clients.end())
	{
		if (command == "JOIN")
			handleJoin(getWord(buffer, 2), it->getNickname(), fd);
		else if (command == "NICK")
			changeNickName(getWord(buffer, 2), it->getNickname());
		else if (command == "INVITE")
			inviteNick(it->getNickname(), getWord(buffer, 2), getWord(buffer, 3), fd);
		else if (command == "LIST")
			listChannels(it->getNickname(), fd);
		else if (command == "PART")
			partChannel(it->getNickname(), getWord(buffer, 2), fd);
		else if (command == "TOPIC")
			topicChannel(getWord(buffer, 2), fd, getWord(buffer, 3));
		else if (command == "NAMES")
			getNamesInChannel(getWord(buffer, 2), fd);
		else if (command == "NOTICE")
			sendNotice(getWord(buffer, 3), getWord(buffer, 2), it->getNickname(), fd);
		else if (command == "PING")
			pingCheck(fd);
		else if (command == "PONG")
			pongCheck(fd, getWord(buffer, 2));
		else if (command == "WHO")
			usersOnNetwork(getWord(buffer, 2), fd);
		else if (command == "WHOIS")
			getUserInfo(getWord(buffer, 2), fd);
		else if (command == "ISON")
			getSpecificUsersInfo(buffer, fd);
		else if (command == "PRIVMSG")
		{
			// no funciona correctamente, tiene por default a un canal, test
			privMessage(buffer, fd);
		}
	}
}
