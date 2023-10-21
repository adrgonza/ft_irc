#include <libraries.hpp>
#include "Client.hpp"
#include "../Channel/Channel.hpp"

Client::Client() {}
Client::~Client() {}

Client &Client::operator=(const Client &obj)
{
	this->_fd = obj.getFd();
	this->_firstTime = obj.getFirsTime();
	this->_nickname = obj.getNickname();
	return *this;
}

bool Client::operator==(const Client &other) const
{
	if (_fd == other.getFd() && this->_nickname == other.getNickname())
		return true;
	else
		return false;
}

Client::Client(const Client &obj)
{
	this->_fd = obj.getFd();
	this->_nickname = obj.getNickname();
	this->_host = obj.getHost();
	this->_passwordkey = obj.getKey();
	this->_channel = obj.getChannel();
	this->_username = obj.getUsername();
	this->_firstTime = obj.getFirsTime();
	this->_realname = obj.getRealName();
}

Client::Client(const int &connectionFd) : _fd(connectionFd), _passwordkey(false), _firstTime(false) {}

int Client::getFd() const { return this->_fd; }
void Client::setFD(const int &newFD) { _fd = newFD; }
void Client::setFirstTime(const bool &ho) { _firstTime = ho; }
const std::string Client::getNickname() const { return this->_nickname; }
const std::string Client::getChannel() const { return this->_channel; }
const std::string Client::getUsername() const { return this->_username; }
const std::string Client::getRealName() const { return this->_realname; }
const std::string Client::getjoined() const{ return _joined; }
void Client::setjoined(const std::string &str) { _joined = str; }
const std::string Client::getHost() const { return this->_host; }
bool Client::getKey() const { return this->_passwordkey; }

const std::string Client::getSource() const
{
	if (this->_nickname.empty())
		return ":";

	std::string source = ":" + this->_nickname;

	if (!this->_username.empty())
		source += "!" + this->_username;

	if (!this->_host.empty())
		source += "@" + this->_host;

	return (source);
}

void Client::changeNickname(std::vector<Client*> &clients, std::map<std::string, Channel*> &channels, const std::string &newNickname, Client &user)
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
	this->sendToAllClientsWithinChanOfUser(NICK_CMD(_nickname, _nickname, newNickname), channels, user);
	this->_nickname = newNickname;
}

void Client::changeUserName(const std::string &user)
{
	std::cout << "New User: " << user << std::endl;
	if (!_username.empty())
	{
		this->sendMessage(ERR_ALREADYREGISTERED(_nickname));
		return ;
	}
	
	std::string username;
	// fijarse si limechat tambien lo manda asi
	size_t colonPos = user.find(':');
	if (colonPos != std::string::npos)
		username = user.substr(colonPos + 1);
	else
		username = getWord(user, 1);
	std::cout << "username: " << username << std::endl;
	_username = username;

	std::string shouldBeZero = getWord(user, 2);
	std::string shouldBeAsteric = getWord(user, 3);
	std::string realname;
	int i = 3;
	while (!getWord(user, ++i).empty())
		realname += getWord(user, i) + " ";
	if (username.empty())
	{
		this->sendMessage(ERR_NEEDMOREPARAMS(_nickname, "USER"));
		return ;
	}
	_username = username;
	if (!realname.empty())
		_realname = realname;
}

void Client::changeChannel(const std::string &channel) { this->_channel = channel; }

void Client::giveKey(const bool &key) { _passwordkey = key; }

bool Client::getFirsTime() const { return(_firstTime); }

void Client::sendMessage(std::string message) const
{
	std::cout << "Sending: " << message << std::endl;
	message = message + IRC_ENDLINE;
	if (send(_fd, message.c_str(), message.size(), 0) == -1)
		std::cout << "[SERVER] Error. Couldn't send message to FD " << _fd << ". Message: \n\t" << message << std::endl;
}
