#include "../Server/Server.hpp"

void Server::usersOnNetwork(const std::string &body, const Client &user)
{
	std::string target = getWord(body, 1);
	if (target.empty() || (!channelExists(target) && !userExists(target)))
	{
		user.sendMessage(ERR_NOSUCHNICK(target, user.getNickname()));
		return;
	}

	std::string serverName = "ToniWarrior's";
	std::string userNickname = user.getNickname();
	if (channelExists(target))
	{
		Channel *targetChannel = getChannelByName(target);
		std::vector<Client*> channelMembers = targetChannel->getParticipants();
		for (std::vector<Client*>::iterator it = channelMembers.begin(); it != channelMembers.end(); ++it)
		{
			std::string username = "username";
			std::string nickname = (*it)->getNickname();
			std::string host = nickname + "!user@host";
			user.sendMessage(RPL_WHOREPLY(serverName, nickname, target, username, host, serverName, user.getNickname(), "H", "1", user.getNickname()));
		}
	}

	if (userExists(target))
	{
		Client *targetClient = findClientByNickname(target);
		std::string nickname = targetClient->getNickname();
		std::string username = "username";
		std::string hostname = "";
		std::string realname = "realname";
		std::string host = nickname + "!user@host";
		user.sendMessage(RPL_WHOREPLY(serverName, nickname, "", username, host, serverName, user.getNickname(), "H", "0", user.getNickname()));
	}
	user.sendMessage(RPL_ENDOFWHO(serverName, user.getNickname(), target));
}

void Server::getUserInfo(const std::string &body, const Client &user)
{
	std::string target = getWord(body, 1);
	if (!userExists(target))
	{
		user.sendMessage(ERR_NOSUCHNICK(target, user.getNickname()));
		return;
	}

	std::string serverName = "ToniWarrior's";
	std::string invitingUser = user.getNickname();
	std::string host = invitingUser + "!user@host";

	Client *clientObj = findClientByNickname(target);
	if (clientObj)
	{
		user.sendMessage(RPL_WHOISUSER(serverName, invitingUser, target, "", host, invitingUser));
		user.sendMessage(RPL_ENDOFWHOIS(serverName, invitingUser, target));
	}

}

void Server::getPreviouslyUsersInfo(const std::string &body, const Client &user)
{

	std::string serverName = "ToniWarrior's";
	std::string target = getWord(body, 2);

	Client *oldClientTarget = NULL;
	for (std::vector<Client*>::iterator it = _disconnectedClients.begin(); it != _disconnectedClients.end(); ++it)
	{
		std::cout << "nick: " << (*it)->getNickname() << std::endl;
		if ((*it)->getNickname() == target)
			oldClientTarget = (*it);
	}

	std::string host = target + "!user@host";
	if (!oldClientTarget)
		user.sendMessage(ERR_NOSUCHNICK(target, user.getNickname()));
	else
	{
		user.sendMessage(RPL_WHOWASUSER(serverName, target, user.getNickname(), "", host, user.getNickname()));
		user.sendMessage(RPL_ENDOFWHOWAS(serverName,  target, user.getNickname()));
	}

}
