/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dangonza <dangonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 14:54:20 by dangonza          #+#    #+#             */
/*   Updated: 2023/07/28 15:27:14 by dangonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP_
#define CLIENT_HPP_

class Client {
	public:
		Client();
		~Client();
		Client(const Client & obj);
		Client& operator=(const Client& obj);

		Client(int connectionFd);

		int getFd() const;
		int getFd();
		std::string getNickname();
		std::string getSource();

		void sendMessage(std::string message, ...);

		// Commands
		void doNickCommand(std::string body);

	private:
		int fd;
		std::string nickname;
		std::string username;
		std::string host;
};

#endif
