#include <Server.hpp>

Server::~Server() {}

Server::Server(std::string network, int port, std::string passw) : network(network), port(port), passw(passw), nbrClients(0) {}

void Server::handleReceivedData(std::string buff_rx, int fd)
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
			disconnectedClients.push_back(*it);
			clients.erase(it);
			break;
		}
	}
	fds[i].fd = -1;
}

void Server::processClientData(int connfd)
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
				std::cerr << "[SERVER-error]: connfd cannot be read. " << errno << strerror(errno) << std::endl;
			else if (len_rx == 0)
				closingClientSocket(i);
			else
			{
				handleReceivedData(buff_rx, fds[i].fd);
				write(connfd, buff_tx, strlen(buff_tx));
			}
		}
	}
}

int Server::handleClientConnection(int sockfd)
{
	int connfd; /* listening socket and connection socket file descriptors */
	struct sockaddr_in client;
	unsigned int len = sizeof(client); /* length of client address */
	time_t currentTime;
	time_t lastPingTimerCheck = time(NULL);

	while (1)
	{
		currentTime = time(NULL);
		time_t elapsedTimeSinceLastCheck = currentTime - lastPingTimerCheck;
		int timeout = (PING_INTERVAL - elapsedTimeSinceLastCheck) * 1000;
		if (timeout < 0)
			timeout = 0;
		int readySockets = poll(fds, clients.size() + 1, timeout);
		if (readySockets == -1)
		{
			std::cerr << "[SERVER-error]: poll() failed" << std::endl;
			return -1;
		}
		if (fds[0].revents & POLLIN)
		{
			connfd = accept(sockfd, (struct sockaddr *)&client, &len);
			if (connfd < 0)
			{
				std::cerr << "[SERVER-error]: connection not accepted" << errno << strerror(errno) << std::endl;
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
					time_t currentTime = time(NULL);
					addClient("", nickname, connfd, currentTime);
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
					std::cerr << "Error receiving Nickname" << std::endl;
					return -1;
				}
			}
		}
		currentTime = time(NULL);
		if (timeout == 0 || (readySockets > 0 && fds[1].revents & POLLIN))
		{
			for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
			{
				if (currentTime - it->getLastPingTime() >= PING_INTERVAL)
				{
					pingCheck(it->getSocketFd());
					it->changeLastPingTime(currentTime);
				}
			}
			lastPingTimerCheck = currentTime;
		}
		processClientData(connfd);
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
		std::cerr << "[SERVER-error]: socket creation failed." << std::endl;
		return -1;
	}
	else
		cout_msg("[SERVER]: socket succesfully created");

	/* clear structure */
	memset(&servaddr, 0, sizeof(servaddr));
	/* assign IP, SERV_PORT, IPV4 */
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(network.c_str());
	servaddr.sin_port = htons(port);

	/* Bind socket */
	if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
	{
		std::cerr << "[SERVER-error]: socket bind failed. " << errno << strerror(errno) << std::endl;
		return -1;
	}
	else
		cout_msg("[SERVER]: Socket successfully binded \n");

	/* Listen */
	if ((listen(sockfd, BACKLOG)) != 0)
	{
		std::cerr << "[SERVER-error]: socket listen failed. " << errno << strerror(errno) << std::endl;
		return -1;
	}
	else
		std::cout << "[SERVER]: Listening on socket: " << network << ":" << std::to_string(ntohs(servaddr.sin_port)) << std::endl;

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
