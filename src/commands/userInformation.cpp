#include <Server.hpp>

void Server::usersOnNetwork(std::string param, int clientFd)
{
	std::vector<Client>::iterator it = findClientByFd(clientFd);
	if (it == clients.end())
	{
		std::cerr << "Client not found" << std::endl;
		return;
	}

	if (param.empty())
	{
		sendErrorMsgToClient("WHO <mask> expected.", 442, it->getNickname(), clientFd, "");
		return;
	}
	std::string serverName = network;

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
	sendMsgToClient(whoMessage, clientFd);
	// std::string endOfWhoMessage = ":" + serverName + " 315 " + userNickname + " :End of WHO list";
	// retValue = send(clientFd, endOfWhoMessage.c_str(), endOfWhoMessage.size(), 0);
	// if (retValue == -1)
	// 	std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
}

// If the <target> parameter is specified, it SHOULD be a server name or the nick of a user.
// Servers SHOULD send the query to a specific server with that name, or to the server <target> is connected to, respectively
void Server::getUserInfo(std::string buffer, int clientFd)
{
	std::string target = getWord(buffer, 2);
	std::string targetNickname = getWord(buffer, 3);
	std::vector<Client>::iterator it = findClientByFd(clientFd);
	std::string invitingUser = it->getNickname();

	if (target == ":")
	{
		sendErrorMsgToClient("WHOIS [<target>] <nick> expected.", 442, it->getNickname(), clientFd, "");
		return;
	}
	if (target == targetNickname)
		// no server to check on
	// TODO - Use of target
	if (!userExists(targetNickname))
	{
		sendErrorMsgToClient("User doesn't exist", 442, invitingUser, clientFd, targetNickname);
		return;
	}

	std::string hostname = "pepe";
	// std::string hostname = invitingUser.getHostname();
	std::string realname = "pepe";
	// std::string realname = invitingUser.getRealname();

	Client *clientObj = findClientByNickname(targetNickname);
	std::string whoisMessage = ":" + network + " 311 " + targetNickname + " " + targetNickname + " " + invitingUser + " " + hostname + " * :" + realname + "\r\n";
	whoisMessage += ":" + network + " 319 " + targetNickname + " " + targetNickname + " :" + clientObj->getChannel() + "\r\n";
	sendMsgToClient(whoisMessage, clientFd);
	std::string endOfWhoisMessage = ":" + network + " 318 " + targetNickname + " " + targetNickname + " :End of WHOIS list\r\n";
	sendMsgToClient(endOfWhoisMessage, clientFd);
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
			sendErrorMsgToClient("User doesn't exist", 442, it->getNickname(), clientFd, userNick);
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
	sendMsgToClient(whoMessage, clientFd);
}
