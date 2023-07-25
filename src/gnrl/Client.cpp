#include <Client.hpp>

Client::~Client() {}

Client::Client(std::string name, std::string nickname, int socketFd, time_t pingTime) : name(name), nickname(nickname), socketFd(socketFd), channel("test"), lastPingTime(pingTime) {}

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

time_t Client::getLastPingTime() const
{
	return this->lastPingTime;
}

void Client::changeNickname(std::string newNick)
{
	this->nickname = newNick;
}

void Client::changeChannel(std::string channel)
{
	this->channel = channel;
}

void Client::changeLastPingTime(time_t newPingTime)
{
	this->lastPingTime = newPingTime;
}
