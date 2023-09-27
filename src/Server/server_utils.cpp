#include <libraries.hpp>
#include "../src/Client/Client.hpp"
#include "../src/Server/Server.hpp"

e_command parseCommandCode(std::string command)
{
	if (command == "NICK")
		return CMD_NICK;
	if (command == "USER")
		return CMD_USER;
	if (command == "PING")
		return CMD_PING;
	if (command == "PONG")
		return CMD_PONG;
	if (command == "OPER")
		return CMD_OPER;
	if (command == "AUTHENTICATE")
		return CMD_AUTH;
	if (command == "QUIT")
		return CMD_QUIT;
	if (command == "JOIN")
		return CMD_JOIN;
	if (command == "PART")
		return CMD_PART;
	if (command == "TOPIC")
		return CMD_TOPIC;
	if (command == "NAMES")
		return CMD_NAMES;
	if (command == "LIST")
		return CMD_LIST;
	if (command == "KICK")
		return CMD_KICK;
	if (command == "PRIVMSG")
		return CMD_PRIVMSG;
	if (command == "SAY")
		return CMD_SAY;
	if (command == "NOTICE")
		return CMD_NOTICE;
	if (command == "KILL")
		return CMD_KILL;
	if (command == "ERROR")
		return CMD_ERROR;
	if (command == "PASS")
		return CMD_PASS;
	if (command == "CAP")
		return CMD_CAP;
	if (command == "INVITE")
		return CMD_INVITE;
	if (command == "TIME")
		return CMD_TIME;
	if (command == "MODE")
		return CMD_MODE;
	if (command == "WHO")
		return CMD_WHO;
	if (command == "WHOIS")
		return CMD_WHOIS;
	if (command == "WHOWAS")
		return CMD_WHOWAS;
	if (command == "REHASH")
		return CMD_REHASH;
	if (command == "RESTART")
		return CMD_RESTART;
	if (command == "SQUIT")
		return CMD_SQUIT;
	return CMD_UNKNOWN;
}

void Server::handleCommand(Client &caller, std::string command, std::string body)
{
	std::cout << "cmd: " << command << ", " << body << std::endl;
	e_command commandCode = parseCommandCode(command);
	switch (commandCode)
	{

	// Client commands
	case CMD_NICK:
		caller.changeNickname(body);
		break;
	case CMD_USER:
		break;
	case CMD_PASS:
		break;
	case CMD_PRIVMSG:
		privMessage(body, caller);
		break;
	case CMD_SAY:
		sayMsg(body, caller);
		break;
	case CMD_NOTICE:
		noticeMessage(body, caller);
		break;

	// Channel commands
	case CMD_JOIN:
		handleJoin(body, caller);
		break;
	case CMD_LIST:
		listChannels(body, caller);
		break;
	case CMD_PART:
		partChannel(body, caller);
		break;
	case CMD_NAMES:
		getNamesInChannel(body, caller);
		break; // NO FUNCIONA
	case CMD_INVITE:
		inviteNick(body, caller);
		break;
	case CMD_TOPIC:
		topicChannel(body, caller);
		break;

	// Server commands

	// Commands yet to do
	case CMD_PING:
	case CMD_PONG:
	case CMD_OPER:
	case CMD_AUTH:
	case CMD_QUIT:
	case CMD_KICK:
	case CMD_KILL:

	// Not sure if needed
	case CMD_ERROR:
	case CMD_CAP:
	case CMD_TIME:
	case CMD_MODE:
	case CMD_WHO:
	case CMD_WHOIS:
	case CMD_WHOWAS:
	case CMD_REHASH:
	case CMD_RESTART:
	case CMD_SQUIT:
	case CMD_UNKNOWN:
		caller.sendMessage(ERR_UNKNOWNCOMMAND_421, caller.getNickname().c_str(), command.c_str());
		break;
	}
}

bool Server::userExists(std::string nickname)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->getNickname() == nickname)
			return true;
	}
	return false;
}
bool Server::channelExists(std::string channelName)
{
	if (channels.find(channelName) != channels.end())
		return true;
	return false;
}

std::vector<Client>::iterator Server::findClientByFd(int fd)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->getFd() == fd)
			return it;
	}
	return _clients.end();
}

int Server::getClientSocketFdByNickname(const std::string &nickname)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->getNickname() == nickname)
			return it->getFd();
	}
	return -1;
}

Channel *Server::getChannelByName(std::string channelName)
{
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it != channels.end())
		return &(it->second);
	else
		return NULL;
}

Client *Server::findClientByNickname(std::string targetNickname)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->getNickname() == targetNickname)
			return &(*it);
	}
	return NULL;
}

std::string Server::getWord(const std::string &str, int wordNumber)
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
