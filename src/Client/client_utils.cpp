#include <libraries.hpp>
#include "Client.hpp"

std::string Client::getWord(const std::string &str, int wordNumber)
{
	if (str.empty())
		return "";

	std::string::size_type startPos = 0;
	std::string::size_type endPos = 0;
	int currentWord = 0;

	while (currentWord < wordNumber)
	{
		startPos = str.find_first_not_of(' ', endPos);
		if (startPos == std::string::npos)
			return "";
		endPos = str.find(' ', startPos);
		if (endPos == std::string::npos)
			endPos = str.length();
		currentWord++;
	}

	return str.substr(startPos, endPos - startPos);
}

void Client::sendToAllClientsWithinChanOfUser(std::string msg, std::map<std::string, Channel*> channels, Client me)
{
	std::vector<Client*> clientsToSendMsg;
	clientsToSendMsg.push_back(&me);

	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		Channel *chan = (it)->second;
		if (chan->hasParticipant(me))
		{
			const std::vector<Client*> clientsInChan = chan->getParticipants();
			for (std::vector<Client*>::const_iterator et = clientsInChan.begin(); et != clientsInChan.end(); et++)
			{
				bool add = true;
				for (std::vector<Client*>::iterator at = clientsToSendMsg.begin(); at != clientsToSendMsg.end(); at++)
				{
					if ((*at)->getNickname() == (*et)->getNickname())
						add = false;
				}
				if (add)
					clientsToSendMsg.push_back(*et);
			}
		}
	}
	for (std::vector<Client*>::iterator it = clientsToSendMsg.begin(); it != clientsToSendMsg.end(); it++)
		(*it)->sendMessage(msg);
}