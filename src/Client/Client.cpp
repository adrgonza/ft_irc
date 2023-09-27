#include <libraries.hpp>
#include "Client.hpp"

Client::Client() {}
Client::~Client() {}

Client &Client::operator=(const Client &obj)
{
	this->_fd = obj.getFd();
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
	this->username = obj.getUsername();
}

Client::Client(int connectionFd) : _fd(connectionFd), _passwordkey(false) {}

int Client::getFd() const { return this->_fd; }
std::string Client::getNickname() const { return this->nickname; }
std::string Client::getChannel() const { return this->channel; }
std::string Client::getUsername() const { return this->username; }
std::string Client::getHost() const { return this->host; }
bool Client::getKey() const { return this->_passwordkey; }

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

void Client::changeNickname(std::string body)
{
	// Assumes (for now) body is always correct. No collisions, no weird characters, and never empty
	// TODO: Handle NICK command correctly (please)

	this->sendMessage("NICK <nickname>", body.c_str());
	this->nickname = body; // Nickname is changed after the message is sent, to not affect the source of the message (see docs.)
}

void Client::changeChannel(std::string channel)
{
	this->channel = channel;
}

void Client::giveKey(bool key)
{
	_passwordkey = key;
}

// client_utils.cpp
std::string buildClientMessage(std::string message, va_list args);

// message = this->getSource() + " " + buildClientMessage(message, args) + IRC_ENDLINE;
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

time_t Client::getLastPingTime() const
{
	return this->lastPingTime;
}

void Client::changeLastPingTime(time_t newPingTime)
{
	this->lastPingTime = newPingTime;
}