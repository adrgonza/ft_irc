#ifndef CLIENT_HPP_
#define CLIENT_HPP_

# define RESET "\e[0m"
# define RED "\e[1;91m"
# define BLUE "\e[1;34m"

class Client {
	public:
		Client(int connectionFd);

		~Client();

		bool operator==(const Client& other) const;
		Client& operator=(const Client& obj);
		Client(const Client &obj);


		int getFd() const;
		void setFD(int);
		std::string getNickname() const;
		std::string getUsername() const;
		std::string getjoined() const;
		void setjoined(std::string);
		std::string getHost() const;
		std::string getChannel() const;
		bool getKey() const;
		std::string getSource() const;
		time_t		getLastPingTime() const;
		bool getPing() const;
		void setPing(bool);


		void giveKey(bool);

		// Commands
		void sendMessage(std::string message, ...);
		void changeNickname(std::vector<Client> clients, std::string newNick);
		void changeChannel(std::string channel);
		void changeLastPingTime(time_t newPingTime);

	private:
		Client();
		int _fd;
		bool _ping;
		std::string _joined;
		std::string nickname;
		std::string username;
		std::string host;
		std::string channel;
		bool _passwordkey;
		time_t		lastPingTime;
};

#endif
