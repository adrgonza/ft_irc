#include "../server/Server.hpp"

// :invitingUser!user@host INVITE targetUser :#channelName

// Habria  que checkquear si el usuario esta en el canal para poder invitar a otro?
// mensajes de error al cliente ! EJ: std::string sendMessage = ":" + network + " 442 " + it->getNickname() + "#" + channel + ":You're not on that channel" + "\r\n";

void Server::inviteNick(std::string invitingUser, std::string targetUser, std::string channel)
{
	std::string inviteMessage = ":" + invitingUser + "!user@host INVITE " + targetUser + " :" + channel + "\r\n";
	int clientFd = 0;
	if (!userExists(targetUser))
	{
		std::cout << targetUser << " user does not exist" << std::endl;
		return ;
	}
	std::string lookChannel = "#" + channel;
	if (!channelExists(lookChannel))
	{
		std::cout << channel << " channel does not exist" << std::endl;
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
