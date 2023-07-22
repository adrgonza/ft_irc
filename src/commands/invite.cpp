#include "../server/Server.hpp"

// :invitingUser!user@host INVITE targetUser :#channelName

void Server::inviteNick(std::string invitingUser, std::string targetUser, std::string channel)
{
	std::string inviteMessage = ":" + invitingUser + "!user@host INVITE " + targetUser + " :" + channel + "\r\n";
	int clientFd = 0;
	if (!userExists(targetUser))
	{
		std::cout << targetUser << " does not exist" << std::endl;
		return ;
	}
	if (channelExists(channel))
	{
		std::cout << channel << " does not exist" << std::endl;
		return ;
	}
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getNickname() == targetUser)
		{
			clientFd = it->getSocketFd();
			break;
		}
	}
	int retValue = send(clientFd, inviteMessage.c_str(), inviteMessage.size(), 0);
	if (retValue == -1)
	{
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
	}
}
