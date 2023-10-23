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
		std::string _network;
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
		std::vector<Client*>::iterator getClientByFd(const int &);
		void disconnectClient(const int &);
		void handleCommand(Client &, const std::string &, std::string &);
		bool userExists(std::string &);
		bool channelExists(const std::string &);
		Client* findClientByFd(const int &);
		int getClientSocketFdByNickname(const std::string &);
		Channel* getChannelByName(const std::string &);
		Client* findClientByNickname(const std::string &);
		std::string getWord(const std::string &, const int &);

		/* commands */
		void privMessage(std::string &, const Client &);
		void sayMsg(std::string &, const Client &);
		void listChannels(std::string &, const Client &);
		void partChannel(const std::string &, Client &);
		void handleJoin(const std::string &, Client &);
		void getNamesInChannel(const std::string &, const Client &);
		void topicChannel(const std::string &, const Client &);
		void inviteNick(const std::string &, const Client &);
		void kickUser(const std::string &, const Client &);
		void pongCheck(const std::string &, const Client &);
		void pingCheck(const std::string &, const Client &);
		void noticeMessage(const std::string &, const Client &);
		void usersOnNetwork(const std::string &, const Client &);
		void getUserInfo(const std::string &, const Client &);
		void getPreviouslyUsersInfo(const std::string &, const Client &);
		void quitServ(std::string &, const Client &);
		void modeHandler(const std::string &, const Client &);
		void banUser(Channel *, std::string &, const std::string &, const Client &);
		void unbanUser(Channel *, std::string &, const std::string &, const Client &);
		void adminOnlyChan(Channel *, const std::string &, const Client &, bool);
		void setSecretChannel(Channel *chan, const std::string & host, const Client &user, bool);
		void setExternalMsgsChannel(Channel *chan, const std::string & host, const Client &user, bool);
};

#endif
