#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client {
	private:
		std::string name;
		std::string nickname;
		int			socketFd;

	public:
		Client(std::string name, std::string nickname, int socketFd);
		~Client();

		int getSocketFd() const;
		std::string getNickname() const;

		void changeNickname(std::string newNick);
};

#endif
