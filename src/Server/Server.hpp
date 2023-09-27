#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <libraries.hpp>
#include "../Client/Client.hpp"
#include "../Channel/Channel.hpp"

#define BACKLOG 5 // Nº of clients pending to connect w/ server
#define BUFFER_SIZE 512 // Max size of each message (Docs: https://modern.ircdocs.horse/#message-format)

class Server {
	public:
		Server(int, std::string);
		~Server();

		bool run();

	private:
		int _port;
		int _socketFd;
		int _connectionFd;
		struct pollfd _pollFd[BACKLOG + 1];
		std::string _password;

		std::vector<Client> _clients;
		std::vector<Client> _disconnectedClients;
		std::map<std::string, Channel> channels;

		bool handleClientConnections();
		bool handleClientCommunications(size_t);
		bool handleClientInput(Client &caller, std::string message);
		void checkPassword(std::string, Client &);
		std::vector<Client>::iterator getClientByFd(int fd);

		// Utils
		void handleCommand(Client &caller, std::string command, std::string body);
		bool	userExists(std::string nickname);
		bool	channelExists(std::string channelName);
		Client* findClientByFd(int fd);
		int		getClientSocketFdByNickname(const std::string &nickname);
		Channel* getChannelByName(std::string channelName);
		Client* findClientByNickname(std::string targetNickname);
		std::string getWord(const std::string &str, int wordNumber);

		// Commands
		void privMessage(std::string body, Client user);
		void sayMsg(std::string body, Client &user);
		void listChannels(std::string body, Client &user);
		void partChannel(std::string body, Client &user);
		void handleJoin(std::string body, Client &user);
		void getNamesInChannel(std::string body, Client &user);
		void topicChannel(std::string body, Client &user);
		void inviteNick(std::string body, Client &user);
		void usersOnNetwork(std::string body, Client &user);
		void getUserInfo(std::string body, Client &user);
		void getPreviouslyUsersInfo(std::string body, Client &user);
};

#endif
