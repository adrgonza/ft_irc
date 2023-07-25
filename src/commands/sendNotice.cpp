#include <Server.hpp>

// Hay un error que por alguna razon el cliente que usamos interpreta solo el /nootice, debe ser algo que se cambia en el config
void Server::sendNotice(std::string buffer, std::string sender, int clientFd)
{
	std::string target = getWord(buffer, 2);
	std::string message = getWord(buffer, 3);
	bool isChannelMessage = target[0] == '#';
	std::string noticeMessage = ":" + sender + "!user@host NOTICE " + target + " :" + message + "\r\n";
	if (isChannelMessage)
	{
		Client *clientObj = findClientByNickname(sender);
		if (clientObj->getChannel() != target)
		{
			sendErrorMsgToClient("You are not in the channel", 442, sender, clientFd, target);
			return;
		}
		std::map<std::string, Channel>::iterator channelIt = channels.find(target);
		if (channelIt != channels.end())
		{
			Channel channelObj = channelIt->second;
			std::vector<std::string> clientsInChannel = channelIt->second.getParticipants();
			for (size_t i = 0; i < clientsInChannel.size(); ++i)
			{
				std::string nickname = clientsInChannel[i];
				int clientSocketFd = getClientSocketFdByNickname(nickname);
				if (clientSocketFd != -1)
					sendMsgToClient(noticeMessage, clientSocketFd);
			}
		}
	}
	else
	{
		Client *clientObj = findClientByNickname(target);
		if (clientObj != NULL)
			sendMsgToClient(noticeMessage, clientObj->getSocketFd());
		else
			sendErrorMsgToClient("Target client not found", 442, sender, clientFd, target);
	}
}
