#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client {
	private:
		std::string name;
		std::string nickname;
		int			socketFd;
		std::string channel;
		time_t		lastPingTime;

	public:
		Client(std::string name, std::string nickname, int socketFd, time_t pingTime);
		~Client();

		int			getSocketFd() const;
		std::string getNickname() const;
		std::string getChannel() const;
		time_t		getLastPingTime() const;

		void changeNickname(std::string newNick);
		void changeChannel(std::string channel);
		void changeLastPingTime(time_t newPingTime);
};

#endif
