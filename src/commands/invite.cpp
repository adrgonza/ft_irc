#include <Server.hpp>

void Server::inviteNick(std::string buffer, std::string invitingUser, int invitingUserFd)
{
	std::string targetUser = getWord(buffer, 2);
	std::string channel = getWord(buffer, 3);

	if (targetUser == ":" || channel.empty())
	{
		sendErrorMsgToClient("INVITE <nickname> <channel> expected.", 442, invitingUser, invitingUserFd, "");
		return;
	}
	if (channel[0] != '#')
	{
		sendErrorMsgToClient("Channel has to start with #", 442, invitingUser, invitingUserFd, channel);
		return ;
	}
	if (!userExists(targetUser))
	{
		sendErrorMsgToClient("User doesn't exist", 442, invitingUser, invitingUserFd, targetUser);
		return;
	}
	if (!channelExists(channel))
	{
		sendErrorMsgToClient("Channel doesn't exist", 442, invitingUser, invitingUserFd, channel);
		return;
	}
	std::vector<Client>::iterator it = findClientByFd(invitingUserFd);
	if (it->getChannel() != channel)
	{
		sendErrorMsgToClient("You are not in the channel", 442, invitingUser, invitingUserFd, channel);
		return;
	}

	int clientFd = 0;
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getNickname() == targetUser)
		{
			if (it->getChannel() == channel)
			{
				sendErrorMsgToClient("Is already in the channel", 442, invitingUser, invitingUserFd, targetUser);
				return;
			}
			clientFd = it->getSocketFd();
			break;
		}
	}
	std::string inviteMessage = ":" + invitingUser + "!user@host INVITE " + targetUser + " :" + channel + "\r\n";
	sendMsgToClient(inviteMessage, clientFd);
}
