#include "server/Server.hpp"

// ./ircserv [host:port_network:password_network] <port> <password>

int main(int argc, char **argv)
{
	Server *myServer;
	if (argc != 4)
		return (cout_msg("Unexact number of argvs"));
	myServer = new Server(argv[1], argv[2], argv[3]);
	myServer->start();
	delete myServer;
	return (0);
}

    