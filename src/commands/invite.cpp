#include <Server.hpp>

void Server::inviteNick(std::string invitingUser, std::string targetUser, std::string channel, int invitingUserFd)
{
	if (!userExists(targetUser))
	{
		std::string errorMessage = ":" + network + " 442 " + invitingUser + " " + invitingUser + "#" + targetUser + " :User doesn't exist" + "\r\n";
		int retValue = send(invitingUserFd, errorMessage.c_str(), errorMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		return;
	}
	std::string lookChannel = "#" + channel;
	if (!channelExists(lookChannel))
	{
		std::string errorMessage = ":" + network + " 442 " + invitingUser + " " + invitingUser + "#" + channel + " :Channel doesn't exist" + "\r\n";
		int retValue = send(invitingUserFd, errorMessage.c_str(), errorMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		return;
	}
	std::vector<Client>::iterator it = findClientByFd(invitingUserFd);
	if (it->getChannel() != lookChannel)
	{
		std::string errorMessage = ":" + network + " 442 " + invitingUser + " " + invitingUser + "#" + channel + " :You are not in the channel" + "\r\n";
		int retValue = send(invitingUserFd, errorMessage.c_str(), errorMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		return;
	}

	int clientFd = 0;
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getNickname() == targetUser)
		{
			if (it->getChannel() == lookChannel)
			{
				std::string errorMessage = ":" + network + " 442 " + invitingUser + " " + invitingUser + "#" + targetUser + " :Is already in the channel" + "\r\n";
				int retValue = send(invitingUserFd, errorMessage.c_str(), errorMessage.size(), 0);
				if (retValue == -1)
					std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
				return;
			}
			clientFd = it->getSocketFd();
			break;
		}
	}
	std::string inviteMessage = ":" + invitingUser + "!user@host INVITE " + targetUser + " :" + channel + "\r\n";
	int retValue = send(clientFd, inviteMessage.c_str(), inviteMessage.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
}
