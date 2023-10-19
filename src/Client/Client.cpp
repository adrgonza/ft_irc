#include <libraries.hpp>
#include "Client.hpp"
#include "../Channel/Channel.hpp"

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
	this->_realname = obj.getRealName();
}

Client::Client(int connectionFd) : _fd(connectionFd), _passwordkey(false), _firstTime(false) {}

int Client::getFd() const { return this->_fd; }
void Client::setFD(int newFD) { _fd = newFD; }
void Client::setFirstTime(bool ho) { _firstTime = ho; }
std::string Client::getNickname() const { return this->nickname; }
std::string Client::getChannel() const { return this->channel; }
std::string Client::getUsername() const { return this->_username; }
std::string Client::getRealName() const { return this->_realname; }
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

void Client::changeNickname(std::vector<Client*> clients, std::map<std::string, Channel*> channels, std::string newNickname, Client user)
{
	if (newNickname.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_") != std::string::npos)
    {
        this->sendMessage(ERR_ERRONEUSNICKNAME(this->getNickname(), newNickname));
        return;
    }
	std::string lower_target_client;
	std::string::size_type aux;
	std::string lower_nick = newNickname;
	aux = newNickname.length();
	for (std::string::size_type i = 0; i < aux; i++)
		lower_nick[i] = std::tolower(newNickname[i]);
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		lower_target_client = (*it)->getNickname();
		aux = lower_target_client.length();
		for (std::string::size_type i = 0; i < aux; i++)
			lower_target_client[i] = std::tolower(lower_target_client[i]);
		if (lower_target_client == lower_nick)
		{
			if (lower_target_client != newNickname )
				break ;
			std::string oneS = "";
			this->sendMessage(ERR_NICKNAMEINUSE(oneS, newNickname));
			return;
		}
	}
	this->sendToAllClientsWithinChanOfUser(NICK_CMD(nickname, nickname, newNickname), channels, user);
	this->nickname = newNickname;
}

void Client::changeUserName(std::string user)
{
	if (!_username.empty())
	{
		this->sendMessage(ERR_ALREADYREGISTERED(nickname));
		return ;
	}
	std::string username = getWord(user, 1);
	std::string shouldBeZero = getWord(user, 2);
	std::string shouldBeAsteric = getWord(user, 3);
	std::string realname;
	int i = 3;
    while (!getWord(user, ++i).empty())
		realname += getWord(user, i) + " ";
	if (username.empty())
	{
		this->sendMessage(ERR_NEEDMOREPARAMS(nickname, "USER"));
		return ;
	}
	_username = username;
	if (!realname.empty())
		_realname = realname;
}

void Client::changeChannel(std::string channel) { this->channel = channel; }

void Client::giveKey(bool key) { _passwordkey = key; }

bool Client::getFirsTime() const { return(_firstTime); }

void Client::sendMessage(std::string message)
{
	std::cout << "Sending: " << message << std::endl;
	message = message + IRC_ENDLINE;
	if (send(_fd, message.c_str(), message.size(), 0) == -1)
		std::cout << "[SERVER] Error. Couldn't send message to FD " << _fd << ". Message: \n\t" << message << std::endl;
}
