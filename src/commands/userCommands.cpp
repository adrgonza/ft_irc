#include "../Server/Server.hpp"

void Server::quitServ(std::string body, Client &user)
{
    if (body == ":")
        body = "";
    user.sendMessage(QUIT_CMD, user.getNickname().c_str(), body.c_str());
}