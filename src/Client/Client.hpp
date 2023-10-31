#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <libraries.hpp>
#include <iostream>
#include <vector>
#include <map>

class Channel;

#define RESET "\e[0m"
#define RED "\e[1;91m"
#define BLUE "\e[1;34m"

class Client
{
public:
	Client(const int &);
	Client(const Client &);

	~Client();

	bool operator==(const Client &) const;
	Client &operator=(const Client &);

	int getFd() const;
	const std::string getNickname() const;
	const std::string getUsername() const;
	const std::string getRealName() const;
	const std::string getjoined() const;
	const std::string getHost() const;
	const std::string getChannel() const;
	bool getKey() const;
	bool getFirsTime() const;
	const std::string getSource() const;
	const std::string getWord(const std::string &, const int &) const;

	/* commands */
	void sendMessage(std::string) const;
	void changeNickname(std::vector<Client *> &, std::map<std::string, Channel *> &, const std::string &, Client &);
	void changeChannel(const std::string &);
	void changeUserName(const std::string &);
	void giveKey(const bool &);
	void setjoined(const std::string &);
	void setFD(const int &);
	void setFirstTime(const bool &);

	void sendToAllClientsWithinChanOfUser(const std::string &, std::map<std::string, Channel *> &, Client &);

private:
	Client();

	int _fd;
	std::string _joined;
	std::string _nickname;
	std::string _username;
	std::string _realname;
	std::string _host;
	std::string _channel;
	bool _passwordkey;
	bool _firstTime;
};

#endif
