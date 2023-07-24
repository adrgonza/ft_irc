#include <Server.hpp>

void Server::usersOnNetwork(std::string param, int clientFd)
{
	param = "";
	std::string serverName = network;

	std::vector<Client>::iterator it = findClientByFd(clientFd);
	if (it == clients.end())
	{
		std::cerr << "Client not found" << std::endl;
		return;
	}
	Client requestingClient = *it;
	// std::string username = requestingClient.getUsername();
	std::string username = "pepe";
	std::string userNickname = requestingClient.getNickname();
	// std::string hostname = requestingClient.getHostname();
	std::string hostname = "pepe";
	std::string nickname = requestingClient.getNickname();

	// Logic to determine the channel status based on the user's membership in channels...
	std::string channelStatus = "";

	// std::string whoMessage = ":" + serverName + " 352 " + userNickname + " * " + username + " " + hostname + " " +
	// 						 serverName + " " + nickname + " " + channelStatus + " :1 ";
	std::string whoMessage;
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		std::string clientNickname = it->getNickname();
		std::string clientUsername = "pepe";
		// std::string clientUsername = it->getUsername();
		std::string clientHostname = "pepe";
		// std::string clientHostname = it->getHostname();
		std::string clientRealname = "pepe";
		// std::string clientRealname = it->getRealname();
		whoMessage += "\r\n";
		whoMessage += ":" + serverName + " 352 " + userNickname + " * " + clientUsername + " " + clientHostname + " " +
					  serverName + " " + clientNickname + " " + channelStatus + " :1 ";
	}
	whoMessage += "\r\n";
	int retValue = send(clientFd, whoMessage.c_str(), whoMessage.size(), 0);
	if (retValue == -1)
	{
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		return;
	}
	// std::string endOfWhoMessage = ":" + serverName + " 315 " + userNickname + " :End of WHO list";
	// retValue = send(clientFd, endOfWhoMessage.c_str(), endOfWhoMessage.size(), 0);
	// if (retValue == -1)
	// 	std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
}

void Server::getUserInfo(std::string targetNickname, int clientFd)
{
	std::vector<Client>::iterator it = findClientByFd(clientFd);
	std::string invitingUser = it->getNickname();
	if (!userExists(targetNickname))
	{
		std::string errorMessage = ":" + network + " 442 " + invitingUser + " " + invitingUser + "#" + targetNickname + " :User doesn't exist" + "\r\n";
		int retValue = send(clientFd, errorMessage.c_str(), errorMessage.size(), 0);
		if (retValue == -1)
			std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		return;
	}

	std::string hostname = "pepe";
	// std::string hostname = invitingUser.getHostname();
	std::string realname = "pepe";
	// std::string realname = invitingUser.getRealname();

	Client *clientObj = findClientByNickname(targetNickname);
	std::string whoisMessage = ":" + network + " 311 " + targetNickname + " " + targetNickname + " " + invitingUser + " " + hostname + " * :" + realname + "\r\n";
	whoisMessage += ":" + network + " 319 " + targetNickname + " " + targetNickname + " :" + clientObj->getChannel() + "\r\n";
	int retValue = send(clientFd, whoisMessage.c_str(), whoisMessage.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;

	std::string endOfWhoisMessage = ":" + network + " 318 " + targetNickname + " " + targetNickname + " :End of WHOIS list\r\n";
	retValue = send(clientFd, endOfWhoisMessage.c_str(), endOfWhoisMessage.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
}

void Server::getSpecificUsersInfo(std::string buffer, int clientFd)
{
	std::string serverName = network;
	int i = 2;
	std::vector<std::string> usersToAsk;
	std::string userNick;
	userNick = getWord(buffer, i);
	std::vector<Client>::iterator it = findClientByFd(clientFd);
	if (it == clients.end())
	{
		std::cerr << "Client not found" << std::endl;
		return;
	}
	Client requestingClient = *it;
	while (!userNick.empty())
	{
		if (!userExists(userNick))
		{
			std::string errorMessage = ":" + network + " 442 " + it->getNickname() + " " + it->getNickname() + "#" + userNick + " :User doesn't exist" + "\r\n";
			int retValue = send(clientFd, errorMessage.c_str(), errorMessage.size(), 0);
			if (retValue == -1)
				std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		}
		else
			usersToAsk.push_back(userNick);
		i++;
		userNick = getWord(buffer, i);
	}
	if (usersToAsk.size() == 0)
	{
		std::cout << "add users to ask about" << std::endl;
		return;
	}
	std::string whoMessage = "";
	// std::string username = requestingClient.getUsername();
	std::string username = "pepe";
	std::string userNickname = requestingClient.getNickname();
	// std::string hostname = requestingClient.getHostname();
	std::string hostname = "pepe";
	std::string nickname = requestingClient.getNickname();
	// Logic to determine the channel status based on the user's membership in channels...
	std::string channelStatus = "";
	for (std::vector<std::string>::iterator it = usersToAsk.begin(); it != usersToAsk.end(); ++it)
	{
		Client *clientObj = findClientByNickname(*it);
		if (clientObj != NULL)
		{
			std::string clientNickname = clientObj->getNickname();
			std::string clientUsername = "pepe";
			// std::string clientUsername = it->getUsername();
			std::string clientHostname = "pepe";
			// std::string clientHostname = it->getHostname();
			std::string clientRealname = "pepe";
			// std::string clientRealname = it->getRealname();
			whoMessage += "\r\n";
			whoMessage += ":" + serverName + " 352 " + userNickname + " * " + clientUsername + " " + clientHostname + " " +
						  serverName + " " + clientNickname + " " + channelStatus + " :1 ";
		}
	}
	whoMessage += "\r\n";
	int retValue = send(clientFd, whoMessage.c_str(), whoMessage.size(), 0);
	if (retValue == -1)
	{
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
		return;
	}
}
