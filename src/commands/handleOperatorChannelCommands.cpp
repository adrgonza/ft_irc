#include "../Server/Server.hpp"

void Server::kickUser(std::string body, Client &user)
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
        std::vector<Client*> clientsInChannel = toChan->getParticipants();
        for (std::vector<Client*>::iterator it = clientsInChannel.begin(); it != clientsInChannel.end(); ++it)
            (*it)->sendMessage(KICK_CMD(nick, nick, channel, targetUser, restOfBody));
        Client *tarUser = findClientByNickname(targetUser);
        chan->removeParticipant(*tarUser);
        tarUser->changeChannel("");
    }
    else
        user.sendMessage(ERR_CHANOPRIVSNEEDED(nick, channel));
}

void Server::modeHandler(std::string body, Client &user)
{
	std::string modeToHandleB = getWord(body, 2);
	std::string modeToHandle = modeToHandleB;
	if (modeToHandleB.length() >= 2)
		std::string modeToHandle = modeToHandleB.substr(0, 2);
	if (modeToHandle == "+b")
	{
		std::string newBody = getWord(body, 1) + " " + getWord(body, 3);
		banUser(newBody, user);
	}
	else if (modeToHandle == "-b")
	{
		std::string newBody = getWord(body, 1) + " " + getWord(body, 3);
		unbanUser(newBody, user);
	}
}

void Server::banUser(std::string body, Client &user)
{
    std::string channel = getWord(body, 1);
    std::string targetUser = getWord(body, 2);
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
    Client *targetClient = findClientByNickname(targetUser);
    if (chan->isOperator(user))
    {
        if (!chan->isBanned(*targetClient))
        {
            chan->addBan(*targetClient);
            std::string serverName = "ToniWarrior's";
            std::string host = targetUser + "!user@host";
            user.sendMessage(MODE_CMD(nick, channel, "+b"));
        }
        else
            user.sendMessage(ERR_BANNEDFROMCHAN(targetUser, channel));
    }
    else
        user.sendMessage(ERR_CHANOPRIVSNEEDED(nick, channel));
}

void Server::unbanUser(std::string body, Client &user)
{
	std::string channel = getWord(body, 1);
	std::string targetUser = getWord(body, 2);

    if (!channelExists(channel))
    {
        user.sendMessage(ERR_NOSUCHCHANNEL(user.getNickname(), channel));
        return;
    }
    Channel *chan = getChannelByName(channel);
    std::string nick = user.getNickname();
    Client *targetClient = findClientByNickname(targetUser);
    if (chan->isOperator(user))
    {
        if (chan->isBanned(*targetClient))
        {
            chan->removeBan(*targetClient);
            std::string serverName = "ToniWarrior's";
            std::string host = targetUser + "!user@host";
            user.sendMessage(MODE_CMD(nick, channel, "-b"));
        }
        else
            user.sendMessage(ERR_USERALREADYBANNED(targetUser, channel));
    }
    else
        user.sendMessage(ERR_CHANOPRIVSNEEDED(nick, channel));
}
