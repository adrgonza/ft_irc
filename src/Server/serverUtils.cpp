#include <libraries.hpp>
#include "Server.hpp"

e_command parseCommandCode(const std::string &command)
{

	if (command == "NICK")
		return (CMD_NICK);
	if (command == "USER")
		return (CMD_USER);
	if (command == "PING")
		return (CMD_PING);
	if (command == "PONG")
		return (CMD_PONG);
	if (command == "QUIT")
		return (CMD_QUIT);
	if (command == "JOIN")
		return (CMD_JOIN);
	if (command == "PART")
		return (CMD_PART);
	if (command == "TOPIC")
		return (CMD_TOPIC);
	if (command == "NAMES")
		return (CMD_NAMES);
	if (command == "LIST")
		return (CMD_LIST);
	if (command == "KICK")
		return (CMD_KICK);
	if (command == "PRIVMSG")
		return (CMD_PRIVMSG);
	if (command == "SAY")
		return (CMD_SAY);
	if (command == "NOTICE")
		return (CMD_NOTICE);
	if (command == "PASS")
		return (CMD_PASS);
	if (command == "INVITE")
		return (CMD_INVITE);
	if (command == "WHO")
		return (CMD_WHO);
	if (command == "WHOIS")
		return (CMD_WHOIS);
	if (command == "WHOWAS")
		return (CMD_WHOWAS);
	if (command == "MODE")
		return (CMD_MODE);

	return (CMD_UNKNOWN);
}

void Server::handleCommand(Client &caller, const std::string &command, std::string &body)
{
	std::cout << "cmd: " << command << ", " << body << std::endl;
	e_command commandCode = parseCommandCode(command);
	switch (commandCode)
	{

	/* Client commands */
	case CMD_NICK:
		caller.changeNickname(_clients, _channels, body, caller);
		break;
	case CMD_USER:
		caller.changeUserName(body);
	case CMD_PASS:
		break;
	case CMD_PRIVMSG:
		privMessage(body, caller);
		break;
	case CMD_SAY:
		sayMsg(body, caller);
		break;

	/* Channel commands */
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
		break;
	case CMD_INVITE:
		inviteNick(body, caller);
		break;
	case CMD_TOPIC:
		topicChannel(body, caller);
		break;
	case CMD_NOTICE:
		noticeMessage(body, caller);
		break;
	case CMD_QUIT:
		quitServ(body, caller);
		break;
	case CMD_MODE:
		modeHandler(body, caller);
		break;

	/* Server commands */
	case CMD_PING:
		pingCheck(body, caller);
		break;
	case CMD_PONG:
		pongCheck(body, caller);
		break;
	case CMD_WHO:
		usersOnNetwork(body, caller);
		break;
	case CMD_WHOIS:
		getUserInfo(body, caller);
		break;
	case CMD_WHOWAS:
		getPreviouslyUsersInfo(body, caller);
		break;
	case CMD_KICK:
		kickUser(body, caller);
		break;
	case CMD_UNKNOWN:
		caller.sendMessage(ERR_UNKNOWNCOMMAND_421(caller.getNickname(), command));
		break;
	}
}

bool Server::userExists(std::string &nickname)
{
	std::string lclient;
	std::string::size_type aux = lclient.length();
	aux = nickname.length();
	for (std::string::size_type i = 0; i < aux; i++)
		nickname[i] = std::tolower(nickname[i]);
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		lclient = (*it)->getNickname();
		aux = lclient.length();
		for (std::string::size_type i = 0; i < aux; i++)
			lclient[i] = std::tolower(lclient[i]);
		if (lclient == nickname)
			return (true);
	}
	return (false);
}

bool Server::channelExists(const std::string &channelName)
{
	if (_channels.find(channelName) != _channels.end())
		return (true);
	return (false);
}

Client *Server::findClientByFd(const int &fd)
{
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		if ((*it)->getFd() == fd)
			return (*it);
	return (NULL);
}

int Server::getClientSocketFdByNickname(const std::string &nickname)
{
	std::string lclient;
	std::string lnickname = nickname;
	std::string::size_type aux;
	aux = lnickname.length();
	for (std::string::size_type i = 0; i < aux; i++)
		lnickname[i] = std::tolower(nickname[i]);
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		lclient = (*it)->getNickname();
		aux = lclient.length();
		for (std::string::size_type i = 0; i < aux; i++)
			lclient[i] = std::tolower(lclient[i]);
		if (lclient == lnickname)
			return ((*it)->getFd());
	}
	return (-1);
}

Channel *Server::getChannelByName(const std::string &channelName)
{
	std::map<std::string, Channel *>::iterator it = _channels.find(channelName);
	if (it != _channels.end())
		return (it->second);
	else
		return (NULL);
}

std::vector<Client *>::iterator Server::getClientByFd(const int &fd)
{
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
		if ((*it)->getFd() == fd)
			return (it);
	return (_clients.end());
}

Client *Server::findClientByNickname(const std::string &targetNickname)
{
	std::string lclient;
	std::string lnickname = targetNickname;
	std::string::size_type aux;
	aux = lnickname.length();
	for (std::string::size_type i = 0; i < aux; i++)
		lnickname[i] = std::tolower(targetNickname[i]);

	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		lclient = (*it)->getNickname();
		aux = lclient.length();
		for (std::string::size_type i = 0; i < aux; i++)
			lclient[i] = std::tolower(lclient[i]);
		if (lclient == lnickname)
			return (*it);
	}
	return (NULL);
}

std::string Server::getWord(const std::string &str, const int &wordNumber)
{
	if (str.empty())
		return ("");

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

	return (str.substr(startPos, endPos - startPos));
}

void Server::disconnectClient(const int &i)
{
	std::cout << "[SERVER]: A Client was disconnected from the server" << std::endl;
	Client *deletedClient = findClientByFd(_pollFds[i].fd);
	if (deletedClient)
	{
		close(deletedClient->getFd());
		for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
			if ((*it)->getFd() == deletedClient->getFd())
			{
				_disconnectedClients.push_back(deletedClient);
				_clients.erase(it);
				break;
			}
		}
	}
	_pollFds[i].fd = -1;
	_pollFds[i].revents = 0;
}

void Server::handleCap(const Client &user)
{
	user.sendMessage(RPL_CAP());
}
