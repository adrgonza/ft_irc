#include "../server/Server.hpp"

// Hay un error que por alguna razon el cliente que usamos interpreta solo el /nootice, debe ser algo que se cambia en el config
void Server::sendNotice(std::string message, std::string target, std::string sender, int clientFd)
{
	bool isChannelMessage = target[0] == '#';
	std::string noticeMessage = ":" + sender + "!user@host NOTICE " + target + " :" + message + "\r\n";
	if (isChannelMessage)
	{
		Client *clientObj = findClientByNickname(sender);
		if (clientObj->getChannel() != target)
		{
			std::string errorMessage = ":" + network + " 442 " + sender + " " + sender + " " + target + " :You are not in that channel" + "\r\n";
			int retValue = send(clientFd, errorMessage.c_str(), errorMessage.size(), 0);
			if (retValue == -1)
				std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
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
				{
					int retValue = send(clientSocketFd, noticeMessage.c_str(), noticeMessage.size(), 0);
					if (retValue == -1)
						std::cerr << "[SERVER-error]: send failed for " << nickname << ": " << errno << strerror(errno) << std::endl;
				}
			}
		}
	}
	else
	{
		Client *clientObj = findClientByNickname(target);
		if (clientObj != NULL)
		{
			int retValue = send(clientObj->getSocketFd(), noticeMessage.c_str(), noticeMessage.size(), 0);
			if (retValue == -1)
				std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		}
		else
		{
			std::string errorMessage = ":" + network + " 442 " + sender + " " + sender + " " + target + " :Target client not found" + "\r\n";
			int retValue = send(clientFd, errorMessage.c_str(), errorMessage.size(), 0);
			if (retValue == -1)
				std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
			return;
		}
	}
}
