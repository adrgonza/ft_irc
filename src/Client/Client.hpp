#ifndef CLIENT_HPP_
#define CLIENT_HPP_

class Client {
	public:
		Client(int connectionFd);

		~Client();

		bool operator==(const Client& other) const;

		int getFd() const;
		std::string getNickname() const;
		std::string getSource();
		std::string getChannel() const;
		void giveKey(bool);
		bool getKey();

		// Commands
		void sendMessage(std::string message, ...);
		void changeNickname(std::string newNick);
		void changeChannel(std::string channel);

	private:
		int _fd;
		std::string nickname;
		std::string username;
		std::string host;
		std::string channel;
		bool _passwordkey;
};

#endif
