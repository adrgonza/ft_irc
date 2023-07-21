#include "Client.hpp"

Client::~Client() {}

Client::Client(std::string name, std::string nickname, int socketFd): name(name), nickname(nickname), socketFd(socketFd) {}


int Client::getSocketFd() const {
	return this->socketFd;
}

std::string Client::getNickname() const {
	return this->nickname;
}
