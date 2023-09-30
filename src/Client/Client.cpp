#include <libraries.hpp>
#include "Client.hpp"

Client::Client() {}
Client::~Client() {}

Client &Client::operator=(const Client &obj)
{
	this->_fd = obj.getFd();
	_ping = obj.getPing();
	this->nickname = obj.getNickname();
	return *this;
}

bool Client::operator==(const Client &other) const
{
	if (_fd == other.getFd() && this->nickname == other.getNickname() && _ping == other.getPing())
		return true;
	else
		return false;
}

Client::Client(const Client &obj)
{
	this->_fd = obj.getFd();
	this->nickname = obj.getNickname();
	this->host = obj.getHost();
	this->_passwordkey = obj.getKey();
	this->channel = obj.getChannel();
	this->username = obj.getUsername();
	_ping = obj.getPing();
}

Client::Client(int connectionFd) : _fd(connectionFd), _ping(false), _passwordkey(false) {}

int Client::getFd() const { return this->_fd; }
void Client::setFD(int newFD) { _fd = newFD; }
std::string Client::getNickname() const { return this->nickname; }
std::string Client::getChannel() const { return this->channel; }
std::string Client::getUsername() const { return this->username; }
std::string Client::getjoined() const{ return _joined; }
void Client::setjoined(std::string str) { _joined = str; }
std::string Client::getHost() const { return this->host; }
bool Client::getKey() const { return this->_passwordkey; }
bool Client::getPing() const
{
	if(_ping == true)
		return (true);
	return false;
}
void Client::setPing(bool tof) { _ping = tof; }


std::string Client::getSource() const
{
	if (this->nickname.empty())
		return ":";

	std::string source = ":" + this->nickname;

	if (!this->username.empty())
		source += "!" + this->username;

	if (!this->host.empty())
		source += "@" + this->host;

	return source;
}

void Client::changeNickname(std::vector<Client> clients, std::string body)
{
	std::string lbody;
	std::string lclient;
	std::string::size_type aux;
	aux = lbody.length();
    for (std::string::size_type i = 0; i < aux; i++)
        lbody[i] = std::tolower(lbody[i]);



	lbody = body;
	aux = lbody.length();
    for (std::string::size_type i = 0; i < aux; i++)
    	lbody[i] = std::tolower(lbody[i]);
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		lclient = it->getNickname();
		aux = lclient.length();
		for (std::string::size_type i = 0; i < aux; i++)
       		lclient[i] = std::tolower(lclient[i]);
		if (lclient == lbody)
		{
			this->sendMessage(ERR_NICKNAMEINUSE("", body));
			return;
		}
	}
	this->sendMessage(NICK_CMD(body));
	this->nickname = body;
}

void Client::changeChannel(std::string channel)
{
	this->channel = channel;
}

void Client::giveKey(bool key)
{
	_passwordkey = key;
}

void Client::sendMessage(std::string message)
{
	std::cout << "Sending: " << message << std::endl;
	message = message + IRC_ENDLINE;
	if (send(_fd, message.c_str(), message.size(), 0) == -1)
		std::cout << "[SERVER] Error. Couldn't send message to FD " << _fd << ". Message: \n\t" << message << std::endl;
}

time_t Client::getLastPingTime() const
{
	return this->lastPingTime;
}

void Client::changeLastPingTime(time_t newPingTime)
{
	this->lastPingTime = newPingTime;
}
