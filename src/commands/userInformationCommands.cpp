#include "../Server/Server.hpp"

void Server::usersOnNetwork(std::string body, Client &user)
{
    std::string target = getWord(body, 1);
    if (target.empty() || (!channelExists(target) && !userExists(target)))
    {
        user.sendMessage(ERR_NOSUCHNICK, target.c_str(), user.getNickname().c_str());
        return;
    }

    std::string serverName = "ToniWarrior's";
    std::string userNickname = user.getNickname();

    if (channelExists(target))
    {
        Channel *targetChannel = getChannelByName(target);
        std::vector<Client> channelMembers = targetChannel->getParticipants();
        for (std::vector<Client>::iterator it = channelMembers.begin(); it != channelMembers.end(); ++it)
        {
            std::string username = "username"; // Replace with the username if available, or use an empty strin
            std::string nickname = it->getNickname();
            std::string host = nickname + "!user@host";
            user.sendMessage(RPL_WHOREPLY, serverName.c_str(), nickname.c_str(), target.c_str(), username.c_str(), host.c_str(), serverName.c_str(), user.getNickname().c_str(), "H", "1", user.getNickname().c_str());
        }
    }

    if (userExists(target))
    {
        Client *targetClient = findClientByNickname(target);
        std::string nickname = targetClient->getNickname();
        std::string username = "username"; // Replace with the username if available, or use an empty string
        std::string hostname = "";         // Replace with the hostname if available, or use an empty string
        std::string realname = "realname"; // Replace with the realname if available, or use an empty string
        std::string host = nickname + "!user@host";
        user.sendMessage(RPL_WHOREPLY, serverName.c_str(), nickname.c_str(), "", username.c_str(), host.c_str(), serverName.c_str(), user.getNickname().c_str(), "H", "0", user.getNickname().c_str());
    }
    user.sendMessage(RPL_ENDOFWHO, serverName.c_str(), user.getNickname().c_str(), target.c_str());
}

void Server::getUserInfo(std::string body, Client &user)
{
    std::string target = getWord(body, 1);
    if (!userExists(target))
    {
        user.sendMessage(ERR_NOSUCHNICK, target.c_str(), user.getNickname().c_str());
        return;
    }

    std::string serverName = "ToniWarrior's";
    std::string invitingUser = user.getNickname();
    std::string host = invitingUser + "!user@host";

    Client *clientObj = findClientByNickname(target);
    if (clientObj)
    {
        user.sendMessage(RPL_WHOISUSER, serverName.c_str(), invitingUser.c_str(), target.c_str(), "", host.c_str(), invitingUser.c_str());
        user.sendMessage(RPL_ENDOFWHOIS, serverName.c_str(), invitingUser.c_str(), target.c_str());
    }
}

void Server::getPreviouslyUsersInfo(std::string body, Client &user)
{
    std::string serverName = "ToniWarrior's";
    std::string target = getWord(body, 2);
    
    Client *oldClientTarget = NULL;
    for (std::vector<Client>::iterator it = _disconnectedClients.begin(); it != _disconnectedClients.end(); ++it)
    {
        std::cout << "nick: " << it->getNickname() << std::endl;
        if (it->getNickname() == target)
            oldClientTarget = &(*it);
    }
    
    std::string host = target + "!user@host";
    if (!oldClientTarget)
        user.sendMessage(ERR_NOSUCHNICK, target.c_str(), user.getNickname().c_str());
    else
    {
        user.sendMessage(RPL_WHOWASUSER, serverName.c_str(), target.c_str(), user.getNickname().c_str(), "", host.c_str(), user.getNickname().c_str());
        user.sendMessage(RPL_ENDOFWHOWAS, serverName.c_str(),  target.c_str(), user.getNickname().c_str());
    }
}
