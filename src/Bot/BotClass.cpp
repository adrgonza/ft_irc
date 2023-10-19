#include "Bot.hpp"

Bot::Bot(const std::string &server, const int &port, const std::string &channel, const std::string &nick, const std::string &user, const std::string &password)
	: _server(server), _port(port), _channel(channel), _nick(nick), _userCmd(user), _password(password) {}

Bot::~Bot(){}

void Bot::setSocket(const int &sock)
{
	this->_sock = sock;
}

int Bot::getSocket() const
{
	return this->_sock;
}

const std::string Bot::getServer() const
{
	return this->_server;
}

int Bot::getPort() const
{
	return this->_port;
}

const std::string Bot::getChannel() const
{
	return this->_channel;
}

const std::string Bot::getNick() const
{
	return this->_nick;
}

const std::string Bot::getUserCmd() const
{
	return this->_userCmd;
}

const std::string Bot::getPassword() const
{
	return this->_password;
}
