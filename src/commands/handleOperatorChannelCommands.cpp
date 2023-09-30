#include "../Server/Server.hpp"

void Server::kickUser(std::string body, Client &user)
{
    std::string channel = getWord(body, 1);
    std::string targetUser = getWord(body, 2);

    if (!channelExists(channel))
    {
        user.sendMessage(ERR_NOSUCHCHANNEL(user.getNickname().c_str(), channel.c_str()));
        return;
    }
    if (!userExists(targetUser))
    {
        user.sendMessage(ERR_NOSUCHNICK(user.getNickname().c_str(), targetUser.c_str()));
        return;
    }
    Channel *chan = getChannelByName(channel);
    std::string nick = user.getNickname();
    if (chan->isOperator(user))
    {
        user.sendMessage(KICK_CMD(nick.c_str(), channel.c_str(), targetUser.c_str()));
        Client *tarUser = findClientByNickname(targetUser);
        chan->removeParticipant(*tarUser);
        tarUser->changeChannel("");
        tarUser->sendMessage(KICK_CMD(nick.c_str(), channel.c_str(), targetUser.c_str()));
    }
    else
        user.sendMessage(ERR_CHANOPRIVSNEEDED(nick.c_str(), channel.c_str()));
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
        user.sendMessage(ERR_NOSUCHCHANNEL(user.getNickname().c_str(), channel.c_str()));
        return;
    }
    if (!userExists(targetUser))
    {
        user.sendMessage(ERR_NOSUCHNICK(user.getNickname().c_str(), targetUser.c_str()));
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
            user.sendMessage(BAN_CMD(serverName.c_str(), channel.c_str(), host.c_str()));
        }
        else
            user.sendMessage(ERR_BANNEDFROMCHAN(targetUser.c_str(), channel.c_str()));
    }
    else
        user.sendMessage(ERR_CHANOPRIVSNEEDED(nick.c_str(), channel.c_str()));
}

void Server::unbanUser(std::string body, Client &user)
{
    std::string channel = getWord(body, 1);
    std::string targetUser = getWord(body, 2);

    if (!channelExists(channel))
    {
        user.sendMessage(ERR_NOSUCHCHANNEL(user.getNickname().c_str(), channel.c_str()));
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
            user.sendMessage(UNBAN_CMD(serverName.c_str(), channel.c_str(), host.c_str()));
        }
        else
            user.sendMessage(ERR_USERALREADYBANNED(targetUser.c_str(), channel.c_str()));
    }
    else
        user.sendMessage(ERR_CHANOPRIVSNEEDED(nick.c_str(), channel.c_str()));
}