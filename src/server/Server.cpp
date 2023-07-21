#include "Server.hpp"

#define SERV_HOST_ADDR "10.11.14.6" /* IP, only IPV4 support  */
#define BACKLOG 5					/* Max. client pending connections  */

Server::~Server() {}

Server::Server(std::string network, std::string port, std::string passw) : network(network), port(port), passw(passw) {}

int Server::start(void)
{
	int sockfd, connfd; /* listening socket and connection socket file descriptors */
	unsigned int len;	/* length of client address */
	struct sockaddr_in servaddr, client;

	int len_rx = 0; /* received and sent length, in bytes */
	char buff_tx[100] = "Hello client, I am the server";
	char buff_rx[100]; /* buffers for reception  */

	/* socket creation */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		cout_msg("[SERVER-error]: socket creation failed.");
		return -1;
	}
	else
		cout_msg("[SERVER]: socket succesfully created");

	/* clear structure */
	memset(&servaddr, 0, sizeof(servaddr));
	/* assign IP, SERV_PORT, IPV4 */
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
	servaddr.sin_port = htons(atoi(this->port.c_str()));

	/* Bind socket */
	if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
	{
		fprintf(stderr, "[SERVER-error]: socket bind failed. %d: %s \n", errno, strerror(errno));
		return -1;
	}
	else
		printf("[SERVER]: Socket successfully binded \n");

	/* Listen */
	if ((listen(sockfd, BACKLOG)) != 0)
	{
		fprintf(stderr, "[SERVER-error]: socket listen failed. %d: %s \n", errno, strerror(errno));
		return -1;
	}
	else
		cout_msg("[SERVER]: Listening on SERV_PORT " + std::to_string(ntohs(servaddr.sin_port)) + "\n");

	len = sizeof(client);

	std::vector<int> clientSockets;
	struct pollfd fds[BACKLOG + 1];
	fds[0].fd = sockfd;
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	for (int i = 1; i <= BACKLOG; ++i)
		fds[i].fd = -1; // Set -1 to indicate an unused entry

	/* Accept the data from incoming sockets in a iterative way */
	while (1)
	{
		int readySockets = poll(fds, clientSockets.size() + 1, -1);
		if (readySockets == -1)
		{
			cout_msg("[SERVER-error]: poll() failed");
			return -1;
		}
		if (fds[0].revents & POLLIN)
		{
			connfd = accept(sockfd, (struct sockaddr *)&client, &len);
			if (connfd < 0)
			{
				cout_msg("[SERVER-error]: connection not accepted");
				// fprintf(stderr, "[SERVER-error]: connection not accepted. %d: %s \n", errno, strerror(errno));
				return -1;
			}
			else
			{
				cout_msg("[SERVER]: a connection has been made");
				clientSockets.push_back(connfd);
				for (unsigned long i = 1; i <= clientSockets.size(); ++i)
				{
					if (fds[i].fd == -1)
					{
						fds[i].fd = connfd;
						fds[i].events = POLLIN;
						break;
					}
				}
			}
		}
		for (unsigned long i = 1; i <= clientSockets.size(); ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				len_rx = read(fds[i].fd, buff_rx, sizeof(buff_rx));
				buff_rx[len_rx] = '\0';
				if (len_rx == -1)
				{
					std::string a = strerror(errno);
					cout_msg("[SERVER-error]: connfd cannot be read. " + std::to_string(errno) + a);
				}
				else if (len_rx == 0)
				{
					cout_msg("[SERVER]: client socket closed \n\n");
					close(fds[i].fd);
					clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), fds[i].fd), clientSockets.end());
					fds[i].fd = -1;
				}
				else
				{
					std::string a = buff_rx;
					cout_msg("[SERVER]: " + a);
					write(connfd, buff_tx, strlen(buff_tx));

					// Handle IRC Commands
					doIrcCommand(a);
				}
			}
		}
	}
	return 0;
}
