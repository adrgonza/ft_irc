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

		void handleClientConnections(int socket);
		void handleClientCommunications();
		void handleClientInput(Client caller, std::string message);

		void disconnectClient(int clientFd);
};

#endif
