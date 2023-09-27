#include "../Server/Server.hpp"

// Habria que ir manteniendo de alguna manera la ultima interaccion del usuario, si falta poco pasar ser hechado porque no interactuo.
// Mandarle un mensaje para que haga el PING

// Cheque lo que devuelve /ping, si el network es el correcto sigue en el servidor (debe responder con pong en un periodo de tiempo)
void Server::pongCheck(std::string body, Client &user)
{
    std::string networkToCheck = body;
    if (networkToCheck != _network)
        // Kill clientFd user
        // clientFd = 0;
        body = user.getNickname();
}

// El commando ping lo deberiamos mandar cada determinado tiempo, para saber si el usuario esta activo o hay que sacarlo del servidor
void Server::pingCheck(std::string body, Client &user)
{
    body = "" + body;
    std::cout << "sending ping" << std::endl;
    std::string pongMessage = "PING :" + _network + "\r\n";
    user.sendMessage(pongMessage);
}
