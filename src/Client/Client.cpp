#include <libraries.hpp>
#include "Client.hpp"

Client::Client() {}
Client::~Client() {}

Client &Client::operator=(const Client &obj)
{
	this->_fd = obj.getFd();
	this->_firstTime = obj.getFirsTime();
	this->nickname = obj.getNickname();
	return *this;
}

bool Client::operator==(const Client &other) const
{
	if (_fd == other.getFd() && this->nickname == other.getNickname())
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
	this->_username = obj.getUsername();
	this->_firstTime = obj.getFirsTime();
}

Client::Client(int connectionFd) : _fd(connectionFd), _passwordkey(false), _firstTime(false) {}

int Client::getFd() const { return this->_fd; }
void Client::setFD(int newFD) { _fd = newFD; }
void Client::setFirstTime(bool ho) { _firstTime = ho; }
std::string Client::getNickname() const { return this->nickname; }
std::string Client::getChannel() const { return this->channel; }
std::string Client::getUsername() const { return this->_username; }
std::string Client::getjoined() const{ return _joined; }
void Client::setjoined(std::string str) { _joined = str; }
std::string Client::getHost() const { return this->host; }
bool Client::getKey() const { return this->_passwordkey; }

std::string Client::getSource() const
{
	if (this->nickname.empty())
		return ":";

	std::string source = ":" + this->nickname;

	if (!this->_username.empty())
		source += "!" + this->_username;

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
	{
		lbody[i] = std::tolower(lbody[i]);
	}
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
			this->sendMessage(ERR_NICKNAMEINUSE, "", body.c_str());
			return;
		}
	}
	this->sendMessage("NICK <nickname>", body.c_str());
	this->nickname = body;
}

void Client::changeUserName(std::string user)
{
	std::istringstream iss(user);
	std::string arg;
	std::string name;

	iss >> name;

	int count = 0;
	while (iss >> arg)
		count++;
	if (count > 2)
	{
		_username = name;
	}
	else
	{
		std::cout << "too few arguments" << std::endl;
		//send few argumment
	}
}

void Client::changeChannel(std::string channel) { this->channel = channel; }

void Client::giveKey(bool key) { _passwordkey = key; }

bool Client::getFirsTime() const { return(_firstTime); }

void Client::sendMessage(std::string message, ...)
{
	va_list args;
	va_start(args, message);
	message = buildClientMessage(message, args) + IRC_ENDLINE;
	va_end(args);

	std::cout << "Sending: " << message << std::endl;

	if (send(_fd, message.c_str(), message.size(), 0) == -1)
		std::cout << "[SERVER] Error. Couldn't send message to FD " << _fd << ". Message: \n\t" << message << std::endl;
}
