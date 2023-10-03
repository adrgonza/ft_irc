#include "../Server/Server.hpp"

void Server::noticeMessage(std::string body, Client &user)
{
	std::istringstream iss(body);
	std::string target;
	std::string lowerTarget;
	std::string message;
	std::string sendMessage;

	iss >> target;
	std::getline(iss, message);
	if (!message.empty() && message[0] == ' ')
		message = message.substr(1);

	lowerTarget = target;
	for (std::string::size_type i = 0; i < target.length(); i++)
		lowerTarget[i] = std::tolower(target[i]);
	if (lowerTarget[0] == '#')
	{
		std::vector<Client>::iterator it;
		for (it = _clients.begin(); it != _clients.end(); ++it)
		{
			if (it->getChannel() == lowerTarget && it->getNickname() != user.getNickname())
				it->sendMessage(NOTICE_CMD(user.getNickname(), target, message));
		}
	}
	else
	{
		int targetSocket = getClientSocketFdByNickname(lowerTarget);
		if (targetSocket == -1)
			return;
		Client *receiver = findClientByNickname(lowerTarget);
		receiver->sendMessage(NOTICE_RECEIVER_CMD(user.getNickname(), target, message));
	}
	return;
}
