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

		struct pollfd	fds[BACKLOG + 1];

		std::vector<Client> clients;
		int 	nbrClients;

		std::map<std::string, std::vector<std::string> > channels;

	public:
		Server(std::string network, std::string port, std::string passw);
		~Server();

		int		start();
		void	processClientData(int connfd);
		int		handleClientConnection(int sockfd);
		void	handleReceivedData(std::string buff_rx, int fd);
		void	addClient(std::string name, std::string nick, int socket);
		void	doIrcCommand(std::string buffer, int fd);
		void	closingClientSocket(int i);

		// Commands
		void	handleJoin(std::string channel, std::string user);
		void	privMessage(std::string buffer, int fd);
};

int cout_msg(std::string msg);
void doIrcCommand(std::string buffer, int fd);
bool isIrcCommand(std::string buffer);
std::string getCommand(std::string buffer);

#endif
