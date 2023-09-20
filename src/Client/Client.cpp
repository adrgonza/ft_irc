#include <libraries.hpp>
#include "Client.hpp"

Client::Client(int connectionFd) : _fd(connectionFd), _passwordkey(false) {}

Client::~Client() {}

bool Client::operator==(const Client& other) const {
	if (_fd == other.getFd() && this->nickname == other.getNickname())
		return true;
	else
		return false;
}


int Client::getFd() const { return _fd; }
std::string Client::getNickname() const { return this->nickname; }
std::string Client::getChannel() const { return this->channel; }

// client_utils.cpp
std::string buildClientMessage(std::string message, va_list args);

void Client::sendMessage(std::string message, ...)
{
	va_list args;
	va_start(args, message);

	// TODO: add client username, host & stuff to the start of the message
	message = this->getSource() + " " + buildClientMessage(message, args) + IRC_ENDLINE;

	va_end(args);

	if (send(_fd, message.c_str(), message.size(), 0) == -1)
		std::cout << "[SERVER] Error. Couldn't send message to FD " << _fd << ". Message: \n\t" << message << std::endl;
}

std::string Client::getSource()
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

bool Client::getKey()
{
	if (_passwordkey == true)
		return (true);
	return(false);
}
