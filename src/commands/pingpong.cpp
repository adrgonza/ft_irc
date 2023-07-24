#include <Server.hpp>

// Habria que ir manteniendo de alguna manera la ultima interaccion del usuario, si falta poco pasar ser hechado porque no interactuo.
// Mandarle un mensaje para que haga el PING

// Cheque lo que devuelve /ping, si el network es el correcto sigue en el servidor (debe responder con pong en un periodo de tiempo)
void Server::pongCheck(int clientFd, std::string networkToCheck)
{
	if (networkToCheck != network)
		// kick user from network
	clientFd = 0;
}

// El commando ping lo deberiamos mandar cada determinado tiempo, para saber si el usuario esta activo o hay que sacarlo del servidor
void	Server::pingCheck(int clientFd)
{
	std::string pongMessage = "PING :" + network + "\r\n";
	int retValue = send(clientFd, pongMessage.c_str(), pongMessage.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
}
