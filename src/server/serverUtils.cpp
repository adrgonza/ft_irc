#include <Server.hpp>

void Server::addClient(std::string name, std::string nick, int socket, time_t currentTime)
{
	std::size_t newlinePos = nick.find('\n');
	std::string extractedNickname = nick.substr(0, newlinePos);
	if (!extractedNickname.empty() && extractedNickname.back() == '\r')
		extractedNickname.erase(extractedNickname.size() - 1);
	const std::string NICK_PREFIX = "NICK ";
	if (nick.substr(0, NICK_PREFIX.size()) == NICK_PREFIX)
		extractedNickname = extractedNickname.substr(NICK_PREFIX.size());
	Client toAdd(name, extractedNickname, socket, currentTime);
	clients.push_back(toAdd);
	std::cout << "[SERVER] client successfully added: " << extractedNickname << std::endl;
}

bool Server::userExists(std::string nickname)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
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
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getSocketFd() == fd)
			return it;
	}
	return clients.end();
}

int Server::getClientSocketFdByNickname(const std::string &nickname)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getNickname() == nickname)
			return it->getSocketFd();
	}
	return -1;
}

Channel* Server::getChannelByName(std::string channelName)
{
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it != channels.end())
		return &(it->second);
	else
		return NULL;
}

Client* Server::findClientByNickname(std::string targetNickname)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getNickname() == targetNickname)
			return &(*it);
	}
	return NULL;
}
