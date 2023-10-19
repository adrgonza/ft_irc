#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <libraries.hpp>
#include <poll.h>


#define BACKLOG 10		// Nº of clients pending to connect w/ server
#define BUFFER_SIZE 512 // Max size of each message (Docs: https://modern.ircdocs.horse/#message-format)
#define PING_INTERVAL 30

class Server {
	public:
		Server(const int &, const std::string &);
		~Server();

		bool run();

	private:
		std::string	_network;
		std::string _password;
		int _port;
		int _socketFd;
		int _connectionFd;
		std::vector<pollfd> _pollFds;
		std::vector<Client*> _clients;
		std::vector<Client*> _disconnectedClients;
		std::map<std::string, Channel*> _channels;

		bool handleClientConnections();
		bool handleClientCommunications(const size_t &);
		bool handleClientInput(Client &, std::string);
		void checkPassword(const std::string &, Client &);

		/* utils */
		std::vector<Client*>::iterator getClientByFd(const int &fd);
		void disconnectClient(const int &);
		void handleCommand(Client &caller, std::string command, std::string body);
		bool	userExists(std::string nickname);
		bool	channelExists(std::string channelName);
		Client* findClientByFd(int fd);
		int		getClientSocketFdByNickname(const std::string &nickname);
		Channel* getChannelByName(std::string channelName);
		Client* findClientByNickname(std::string targetNickname);
		std::string getWord(const std::string &str, int wordNumber);

		/* commands */
		void privMessage(std::string body, Client user);
		void sayMsg(std::string body, Client &user);
		void listChannels(std::string body, Client &user);
		void partChannel(std::string body, Client &user);
		void handleJoin(std::string body, Client &user);
		void getNamesInChannel(std::string body, Client &user);
		void topicChannel(std::string body, Client &user);
		void inviteNick(std::string body, Client &user);
		void kickUser(std::string body, Client &user);
		void pongCheck(std::string body, Client &user);
		void pingCheck(std::string body, Client &user);
		void noticeMessage(std::string body, Client &user);
		void usersOnNetwork(std::string body, Client &user);
		void getUserInfo(std::string body, Client &user);
		void getPreviouslyUsersInfo(std::string body, Client &user);
		void modeHandler(std::string body, Client &user);
		void banUser(std::string body, Client &user);
		void unbanUser(std::string body, Client &user);
		void quitServ(std::string body, Client &user);
};

#endif
