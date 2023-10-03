#include "../Server/Server.hpp"

void Server::quitServ(std::string body, Client &user)
{
	if (body == ":")
		body = "";
	std::string nick = user.getNickname();
	user.sendMessage(QUIT_CMD(nick, nick, body));

	int i = 0;
	while (_pollFd[i].fd != user.getFd())
		i++;

	std::cout << "[SERVER]: A Client was disconnected from the server" << std::endl;
	std::vector<Client>::iterator it = std::find(_clients.begin(), _clients.end(), _pollFd[i].fd);
	close(_pollFd[i].fd);
	if (it != _clients.end())
	{
		_disconnectedClients.push_back(*it);
		_clients.erase(it);
	}
	for (size_t i = 0; i < BACKLOG; i++)
	{
		if (_pollFd[i + 1].fd == _pollFd[i].fd)
			_pollFd[i + 1].fd = -1;
		_pollFd[i].revents = 0;
	}
}
