#ifndef CLIENT_HPP_
# define CLIENT_HPP_

#include <libraries.hpp>
#include <iostream>
#include <vector>
#include <map>

class Channel;


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
		std::string getNickname() const;
		std::string getUsername() const;
		std::string getRealName() const;
		std::string getjoined() const;
		std::string getHost() const;
		std::string getChannel() const;
		bool getKey() const;
		bool getFirsTime() const;
		std::string getSource() const;


		// Commands
		void sendMessage(std::string message);
		void changeNickname(std::vector<Client*> clients, std::map<std::string, Channel*> channels, std::string newNick, Client me);
		void changeChannel(std::string channel);
		void changeUserName(std::string name);
		void giveKey(bool);
		void setjoined(std::string);
		void setFD(int);
		void setFirstTime(bool);

		std::string getWord(const std::string &str, int wordNumber);
		void sendToAllClientsWithinChanOfUser(std::string msg, std::map<std::string, Channel*> channels, Client me);
	
	private:
		Client();
		int _fd;
		std::string _joined;
		std::string nickname;
		std::string _username;
		std::string _realname;
		std::string host;
		std::string channel;
		bool _passwordkey;
		bool _firstTime;
};

#endif
