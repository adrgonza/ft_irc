#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <unistd.h>
#include <poll.h>
#include "../gnrl/Client.hpp"

/* sockets */
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <vector>
#include <map>

#define BACKLOG 5					/* Max. client pending connections  */

class Server {
	private:
		std::string network;
		std::string port;
		std::string passw;

		std::vector<Client> clients;
		int 	nbrClients;

		std::map<std::string, std::vector<std::string> > channels;

	public:
		Server(std::string network, std::string port, std::string passw);
		~Server();

		int		start();
		void	addClient(std::string name, std::string nick, int socket);

		// Commands
		void	handleJoin(std::string channel, std::string user);
};

int cout_msg(std::string msg);
void doIrcCommand(std::string buffer);
bool isIrcCommand(std::string buffer);
std::string getCommand(std::string buffer);

#endif
