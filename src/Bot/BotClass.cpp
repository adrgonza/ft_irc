#include "Bot.hpp"

Bot::Bot(std::string server, int port, std::string channel, std::string nick, std::string user, std::string password)
    : _server(server), _port(port), _channel(channel), _nick(nick), _userCmd(user), _password(password) {}

Bot::~Bot(){}

void Bot::setSocket(int sock)
{
	this->_sock = sock;
}

int Bot::getSocket()
{
	return this->_sock;
}

std::string Bot::getServer()
{
	return this->_server;
}

int Bot::getPort()
{
	return this->_port;
}

std::string Bot::getChannel()
{
	return this->_channel;
}

std::string Bot::getNick()
{
	return this->_nick;
}

std::string Bot::getUserCmd()
{
	return this->_userCmd;
}

std::string Bot::getPassword()
{
	return this->_password;
}