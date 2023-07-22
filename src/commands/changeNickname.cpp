#include "../server/Server.hpp"

// le falta, hace el cambio en la clase del cliente pero no en el chat, el oldNick sigue siendo con el que entra
void Server::changeNickName(std::string newNick, std::string oldNick)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getNickname() == oldNick)
		{
			it->changeNickname(newNick);
			break;
		}
	}
	std::cout << oldNick << " changed its nickname to " << newNick << std::endl;
}
