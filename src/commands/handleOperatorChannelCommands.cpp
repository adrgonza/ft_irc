#include "../Server/Server.hpp"

void Server::kickUser(const std::string &body, const Client &user)
{
	std::string channel = getWord(body, 1);
	std::string targetUser = getWord(body, 2);

	if (!channel.empty() && channel[0] != '#')
		return;
	if (!channelExists(channel))
	{
		user.sendMessage(ERR_NOSUCHCHANNEL(user.getNickname(), channel));
		return;
	}
	if (!userExists(targetUser))
	{
		user.sendMessage(ERR_NOSUCHNICK(user.getNickname(), targetUser));
		return;
	}
	Channel *chan = getChannelByName(channel);
	std::string nick = user.getNickname();
	std::string restOfBody;
	int i = 2;
	while (!getWord(body, ++i).empty())
		restOfBody += getWord(body, i) + " ";
	if (chan->isOperator(user))
	{
		Channel *toChan = getChannelByName(channel);
		std::vector<Client *> clientsInChannel = toChan->getParticipants();
		for (std::vector<Client *>::iterator it = clientsInChannel.begin(); it != clientsInChannel.end(); ++it)
			(*it)->sendMessage(KICK_CMD(nick, nick, channel, targetUser, restOfBody));
		Client *tarUser = findClientByNickname(targetUser);
		chan->removeParticipant(*tarUser);
		chan->removeInviteUser(*tarUser);
		tarUser->changeChannel("");
	}
	else
		user.sendMessage(ERR_CHANOPRIVSNEEDED(nick, channel));
}

// MODES

void Server::banUser(Channel *chan, std::string &targetUser, const std::string &host, const Client &user)
{
	if (!userExists(targetUser))
	{
		user.sendMessage(ERR_NOSUCHNICK(user.getNickname(), targetUser));
		return;
	}
	std::string channel = chan->getName();
	Client *targetClient = findClientByNickname(targetUser);
	if (!chan->isBanned(*targetClient))
	{
		chan->addBan(*targetClient);
		user.sendMessage(MODE_CMD(host, channel, "+b", targetUser));
		targetClient->sendMessage(MODE_CMD(host, channel, "+b", targetUser));
	}
	else
		user.sendMessage(ERR_BANNEDFROMCHAN(targetUser, channel));
}

void Server::unbanUser(Channel *chan, std::string &targetUser, const std::string &host, const Client &user)
{
	if (!userExists(targetUser))
	{
		user.sendMessage(ERR_NOSUCHNICK(user.getNickname(), targetUser));
		return;
	}

	std::string channel = chan->getName();
	Client *targetClient = findClientByNickname(targetUser);
	if (chan->isBanned(*targetClient))
	{
		chan->removeBan(*targetClient);
		user.sendMessage(MODE_CMD(host, channel, "-b", targetUser));
		targetClient->sendMessage(MODE_CMD(host, channel, "-b", targetUser));
	}
	else
		user.sendMessage(ERR_USERALREADYBANNED(targetUser, channel));
}

void Server::adminOnlyChan(Channel *chan, const std::string &host, const Client &user, bool to)
{
	std::string channel = chan->getName();

	if (to)
	{
		chan->setAdminOnly();
		user.sendMessage(MODE_CMD(host, channel, "+A", ""));
		chan->setModes("A");
	}
	else
	{
		chan->removeAdminOnly();
		user.sendMessage(MODE_CMD(host, channel, "-A", ""));
	}
}


void Server::setSecretChannel(Channel *chan, const std::string &host, const Client &user, bool to)
{
	std::string channel = chan->getName();

	chan->setSecrect(to);
	if (to)
	{
		user.sendMessage(MODE_CMD(host, channel, "+s", ""));
		chan->setModes("s");
	}
	else
	{
		user.sendMessage(MODE_CMD(host, channel, "-s", ""));
	}
}

void Server::setExternalMsgsChannel(Channel *chan, const std::string &host, const Client &user, bool to)
{
	std::string channel = chan->getName();

	chan->setAcceptExternalMsgs(to);
	if (to)
	{
		user.sendMessage(MODE_CMD(host, channel, "+n", ""));
		chan->setModes("n");
	}
	else
		user.sendMessage(MODE_CMD(host, channel, "-n", ""));
}








