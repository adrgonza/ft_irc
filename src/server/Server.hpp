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
		std::string	network;
		int			 port;
		std::string passw;

		struct pollfd	fds[BACKLOG + 1];

		std::vector<Client> clients;
		int 	nbrClients;

		// Falta agregar el topic al channel
		std::map<std::string, std::vector<std::string> > channels;

	public:
		Server(std::string network, int port, std::string passw);
		~Server();

		int		start();
		void	processClientData(int connfd);
		int		handleClientConnection(int sockfd);
		void	handleReceivedData(std::string buff_rx, int fd);
		void	addClient(std::string name, std::string nick, int socket);
		void	doIrcCommand(std::string buffer, int fd);
		void	closingClientSocket(int i);

		bool	userExists(std::string nickname);
		bool	channelExists(std::string channelName);
		std::vector<Client>::iterator findClientByFd(int fd);
		int		getClientSocketFdByNickname(const std::string &nickname);

		// Commands
		void	privMessage(std::string buffer, int fd);
		void	changeNickName(std::string newNick, std::string oldNick);
		void	handleJoin(std::string channel, std::string user, int fd);
		void	inviteNick(std::string invitingUser, std::string targetUser, std::string channel);
		void	listChannels(std::string user, int clientFd);
		void	partChannel(std::string user, std::string channel, int clientFd);
		void	topicChannel(std::string channel, int clientFd, std::string newTopic);
		void	getNamesInChannel(std::string channel, int clientFd);
		void	usersOnNetwork(std::string param, int clientFd);
};

int cout_msg(std::string msg);
bool isIrcCommand(std::string buffer);
std::string getCommand(std::string buffer);

#endif
