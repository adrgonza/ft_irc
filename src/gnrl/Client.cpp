#include <Client.hpp>

Client::~Client() {}

Client::Client(std::string name, std::string nickname, int socketFd) : name(name), nickname(nickname), socketFd(socketFd), channel(LIMBO_CHANNEL) {}

int Client::getSocketFd() const
{
	return this->socketFd;
}

std::string Client::getNickname() const
{
	return this->nickname;
}

std::string Client::getChannel() const
{
	return this->channel;
}

void Client::changeNickname(std::string newNick)
{
	this->nickname = newNick;
}

void Client::changeChannel(std::string channel)
{
	this->channel = channel;
}
