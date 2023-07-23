#include "../server/Server.hpp"

// :<server> 352 <user> <channel> <username> <hostname> <server_name> <nickname> <H|G>[*][@|+] :<hopcount> <real_name>
// :<server> 315 <user> <channel> :End of WHO list
void	Server::usersOnNetwork(std::string param, int clientFd)
{
	param = "";
	clientFd += 0;
}
