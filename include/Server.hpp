/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dangonza <dangonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 13:14:29 by dangonza          #+#    #+#             */
/*   Updated: 2023/07/27 20:58:56 by dangonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <libraries.hpp>
#include <Client.hpp>
#include <Channel.hpp>

#define BACKLOG 5 // Nº of clients pending to connect w/ server
#define BUFFER_SIZE 512 // Max size of each message (Docs: https://modern.ircdocs.horse/#message-format)

class Server {
	public:
		Server();
		~Server();
		Server(const Server & obj);
		Server& operator=(const Server& obj);

		// Execute the server
		void run(int port);
		std::vector<Client>::iterator getClientByFd(int fd);

	private:
		struct pollfd	backlogFds[BACKLOG + 1];
		std::vector<Client> clients;
		std::map<std::string, Channel> channels;

		void handleClientConnections(int socket);
		void handleClientCommunications();
		void handleClientInput(Client &caller, std::string message);

		void disconnectClient(int clientFd);


		// Utils
		void handleCommand(Client &caller, std::string command, std::string body);
		bool	userExists(std::string nickname);
		bool	channelExists(std::string channelName);
		std::vector<Client>::iterator findClientByFd(int fd);
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



};

#endif
