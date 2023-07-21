#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <unistd.h>
#include <poll.h>

/* sockets */
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include <vector>


class Server {
	private:
		std::string network;
		std::string port;
		std::string passw;

	public:
		Server(std::string network, std::string port, std::string passw);
		~Server();

		int start();
};

int cout_msg(std::string msg);
void doIrcCommand(std::string buffer);


#endif