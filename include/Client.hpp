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

		bool operator==(const Client& other) const;

		int getFd() const;
		std::string getNickname() const;
		std::string getSource();
		std::string getChannel() const;


		// Commands
		void sendMessage(std::string message, ...);
		void privMessage(std::string body, Client user);
		void changeNickname(std::string newNick);
		void changeChannel(std::string channel);

	private:
		int fd;
		std::string nickname;
		std::string username;
		std::string host;
		std::string channel;
};

#endif
