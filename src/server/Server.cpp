#include "Server.hpp"

#define SERV_HOST_ADDR "10.11.14.6" /* IP, only IPV4 support  */

#include <sys/socket.h>

Server::~Server() {}

Server::Server(std::string network, std::string port, std::string passw) : network(network), port(port), passw(passw), nbrClients(0) {}

void Server::handleReceives(std::string buff_rx, int fd)
{
	cout_msg("[SERVER] (recived): " + buff_rx);
	if (isIrcCommand(buff_rx))
		doIrcCommand(buff_rx, fd);
	else
		privMessage(buff_rx, fd);
}

void Server::closingClientSocket(int i)
{
	cout_msg("[SERVER]: client socket closed \n\n");
	close(fds[i].fd);
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getSocketFd() == fds[i].fd)
		{
			clients.erase(it);
			break;
		}
	}
	fds[i].fd = -1;
}

void Server::serverListeningPoll(int connfd)
{
	int len_rx = 0; /* received and sent length, in bytes */
	char buff_tx[100] = "Hello client, I am the server\r\n";
	char buff_rx[100]; /* buffers for reception  */

	for (unsigned long i = 1; i <= clients.size(); ++i)
	{
		if (fds[i].revents & POLLIN)
		{
			len_rx = read(fds[i].fd, buff_rx, sizeof(buff_rx));
			buff_rx[len_rx] = '\0';
			if (len_rx == -1)
				std::cout << "[SERVER-error]: connfd cannot be read. " << errno << strerror(errno) << std::endl;
			else if (len_rx == 0)
				closingClientSocket(i);
			else
			{
				handleReceives(buff_rx, fds[i].fd);
				// connfd or socketfd ?
				write(connfd, buff_tx, strlen(buff_tx));
			}
		}
	}
}

int	Server::handleClientConnection(int sockfd)
{
	int connfd; /* listening socket and connection socket file descriptors */
	struct sockaddr_in client;
	unsigned int len = sizeof(client);	/* length of client address */

	while (1)
	{
		int readySockets = poll(fds, clients.size() + 1, -1);
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
				std::cout << "[SERVER-error]: connection not accepted" << errno << strerror(errno) << std::endl;
				return -1;
			}
			else
			{
				cout_msg("[SERVER]: a connection has been made");
				char nicknameBuffer[256]; // Adjust the buffer size as needed
				ssize_t receivedBytes = recv(connfd, nicknameBuffer, sizeof(nicknameBuffer) - 1, 0);
				if (receivedBytes > 0)
				{
					nicknameBuffer[receivedBytes] = '\0';
					std::string nickname(nicknameBuffer);
					addClient("", nickname, connfd);
					nbrClients += 1;
					for (unsigned long i = 1; i <= clients.size(); ++i)
					{
						if (fds[i].fd == -1)
						{
							fds[i].fd = connfd;
							fds[i].events = POLLIN;
							break;
						}
					}
				}
				else
				{
					cout_msg("Error receiving Nickname");
					return -1;
				}
			}
		}
		serverListeningPoll(connfd);
	}
	return (0);
}

int Server::start(void)
{
	int sockfd;
	struct sockaddr_in servaddr;

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
		std::cout << "[SERVER-error]: socket bind failed. " << errno << strerror(errno) << std::endl;
		return -1;
	}
	else
		cout_msg("[SERVER]: Socket successfully binded \n");

	/* Listen */
	if ((listen(sockfd, BACKLOG)) != 0)
	{
		std::cout << "[SERVER-error]: socket listen failed. " << errno << strerror(errno) << std::endl;
		return -1;
	}
	else
		std::cout << "[SERVER]: Listening on socket: " << SERV_HOST_ADDR << ":" << std::to_string(ntohs(servaddr.sin_port)) << std::endl;

	fds[0].fd = sockfd;
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	for (int i = 1; i <= BACKLOG; ++i)
		fds[i].fd = -1; // Set -1 to indicate an unused entry

	/* Accept the data from incoming sockets in a iterative way */
	if (handleClientConnection(sockfd))
		return 1;
	else
		return -1;
}

void Server::addClient(std::string name, std::string nick, int socket)
{
	std::size_t newlinePos = nick.find('\n');
	std::string extractedNickname = nick.substr(0, newlinePos);
	if (!extractedNickname.empty() && extractedNickname.back() == '\r')
		extractedNickname.erase(extractedNickname.size() - 1);
	const std::string NICK_PREFIX = "NICK ";
	if (nick.substr(0, NICK_PREFIX.size()) == NICK_PREFIX)
		extractedNickname = extractedNickname.substr(NICK_PREFIX.size());
	Client toAdd(name, extractedNickname, socket);
	clients.push_back(toAdd);
	std::cout << "[SERVER] client successfully added: " << extractedNickname << std::endl;
}
