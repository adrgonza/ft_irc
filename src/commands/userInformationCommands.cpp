#include "../Server/Server.hpp"

// The mask can be one of the following:
// A channel name, in which case the channel members are listed.
// An exact nickname, in which case a single user is returned.
// A mask pattern, in which case all visible users whose nickname matches are listed. Servers MAY match other user-specific values, such as the hostname, server, real name or username. Servers MAY not support mask patterns and return an empty list.
void Server::usersOnNetwork(std::string body, Client &user)
{
    std::string target = getWord(body, 1);
    if (target.empty() || (!channelExists(target) && !userExists(target)))
    {
        user.sendMessage(ERR_NOSUCHNICK, target.c_str(), user.getNickname().c_str());
        return;
    }

    std::string serverName = "Toni Warrior's"; // Replace with your server's name
    std::string userNickname = user.getNickname();

    if (channelExists(target))
    {
        Channel *targetChannel = getChannelByName(target);
        std::vector<Client> channelMembers = targetChannel->getParticipants();
        for (std::vector<Client>::iterator it = channelMembers.begin(); it != channelMembers.end(); ++it)
        {
            std::string nickname = it->getNickname();
            std::string whoResponse = ":" + serverName + " 352 " + userNickname + " " + target + " " + nickname + " " + serverName + " " + serverName + " H :1 " + nickname;
	        whoResponse += "\r\n";
            user.sendMessage(whoResponse.c_str());
        }
    }

    if (userExists(target))
    {
        Client *targetClient = findClientByNickname(target);
        std::string nickname = targetClient->getNickname();
        std::string username = ""; // Replace with the username if available, or use an empty string
        std::string hostname = ""; // Replace with the hostname if available, or use an empty string
        std::string realname = ""; // Replace with the realname if available, or use an empty string

        std::string whoResponse = ":" + serverName + " 352 " + userNickname + " " + target + " " + serverName + " " + nickname + " H :1 " + realname;
	    whoResponse += "\r\n";
        user.sendMessage(whoResponse.c_str());
    }

    std::string endOfWhoMessage = ":" + serverName + " 315 " + userNickname + " :End of WHO list";
	endOfWhoMessage += "\r\n";
    user.sendMessage(endOfWhoMessage.c_str());
}

// If the <target> parameter is specified, it SHOULD be a server name or the nick of a user.
// Servers SHOULD send the query to a specific server with that name, or to the server <target> is connected to, respectively
void Server::getUserInfo(std::string body, Client &user)
{
    std::cout << "body: " << body << std::endl;
    std::string target = getWord(body, 2);
    std::string targetNickname = getWord(body, 3);
    std::string invitingUser = user.getNickname();

    // if (target == ":")
    // {
    // 	sendErrorMsgToClient("WHOIS [<target>] <nick> expected.", 442, it->getNickname(), clientFd, "");
    // 	return;
    // }
    if (!userExists(targetNickname))
    {
        // sendErrorMsgToClient("User doesn't exist", 442, invitingUser, clientFd, targetNickname);
        return;
    }

    std::string hostname = "pepe";
    // std::string hostname = invitingUser.getHostname();
    std::string realname = "pepe";
    // std::string realname = invitingUser.getRealname();
    std::string network = "127";

    Client *clientObj = findClientByNickname(targetNickname);
    std::string whoisMessage = ":" + network + " 311 " + targetNickname + " " + targetNickname + " " + invitingUser + " " + hostname + " * :" + realname + "\r\n";
    whoisMessage += ":" + network + " 319 " + targetNickname + " " + targetNickname + " :" + clientObj->getChannel() + "\r\n";
    user.sendMessage(whoisMessage);
    std::string endOfWhoisMessage = ":" + network + " 318 " + targetNickname + " " + targetNickname + " :End of WHOIS list\r\n";
    user.sendMessage(endOfWhoisMessage);
}

// //	Command: WHOWAS
// //	Parameters: <nick> [<count>]
void Server::getPreviouslyUsersInfo(std::string body, Client &user)
{
    std::string nick = getWord(body, 2);
    std::string countStr = getWord(body, 3);
    if (nick.empty())
    {
        // sendErrorMsgToClient("WHOWAS <nick> [<count>] expected.", 442, it->getNickname(), clientFd, "");
        return;
    }
    std::vector<Client>::iterator previousClient;
    std::vector<Client> returnClients;
    int count;
    if (countStr.empty())
        count = std::atoi(countStr.c_str());
    else
        count = -1;

    if (count > 0)
    {
        for (std::vector<Client>::iterator it = _disconnectedClients.begin(); it != _disconnectedClients.end(); ++it)
        {
            if (previousClient->getNickname() == nick)
            {
                returnClients.push_back(*previousClient);
                if (--count == 0)
                    break;
            }
        }
    }
    else
    {
        for (std::vector<Client>::iterator it = _disconnectedClients.begin(); it != _disconnectedClients.end(); ++it)
        {
            if (previousClient->getNickname() == nick)
                returnClients.push_back(*previousClient);
        }
    }

    if (returnClients.size() == 0)
    {
        // sendErrorMsgToClient("No user/s found", 442, it->getNickname(), clientFd, nick);
    }
    std::string network = "127";
    std::string sendMessage = "PREVIOUS USERS INFO: ";
    for (std::vector<Client>::iterator it = _disconnectedClients.begin(); it != _disconnectedClients.end(); ++it)
        // sendMessage += "Nick: " + previousClient->getNickname() + ", Address: " + network + ", LastPingTime: " + std::to_string(previousClient->getLastPingTime()) + "\n";
        sendMessage += "Nick: " + previousClient->getNickname() + ", Address: " + network + ", LastPingTime: " + "1" + "\n";
    user.sendMessage(sendMessage);
}
