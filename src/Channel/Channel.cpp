#include "Channel.hpp"

Channel::Channel(const std::vector<Client *> &clients) : _participants(clients), _topic(""), _adminOnlyChan(false), _isSecrect(false) {}

Channel::~Channel() {}

Channel::Channel(const Channel &obj) : _participants(obj._participants), _topic(obj._topic)
{
	this->_operators = obj._operators;
	this->_name = obj._name;
	this->_bannedParticipants = obj._bannedParticipants;
	this->_adminOnlyChan = obj._adminOnlyChan;
}

const std::vector<Client *> Channel::getParticipants() const
{
	return this->_participants;
}

const std::string Channel::getTopic() const
{
	return this->_topic;
}

void Channel::setTopic(const std::string &newTopic)
{
	this->_topic = newTopic;
}

void Channel::addParticipant(Client &newParticipant)
{
	_participants.push_back(&newParticipant);
}

void Channel::removeParticipant(const Client &participant)
{
	std::vector<Client *>::iterator userIt;
	for (userIt = _participants.begin(); userIt != _participants.end(); ++userIt)
	{
		if ((*userIt)->getNickname() == participant.getNickname())
		{
			_participants.erase(userIt);
			return;
		}
	}
}

bool Channel::hasParticipant(const Client &participant)
{
	std::vector<Client *>::iterator userIt;
	for (userIt = _participants.begin(); userIt != _participants.end(); ++userIt)
	{
		if ((*userIt)->getNickname() == participant.getNickname())
			return true;
	}
	return false;
}

const std::vector<Client *> Channel::getOperators() const
{
	return this->_operators;
}

void Channel::addOperator(Client &newOper)
{
	_operators.push_back(&newOper);
}

void Channel::removeOperator(const Client &oper)
{
	std::vector<Client *>::iterator userIt;
	for (userIt = _operators.begin(); userIt != _operators.end(); ++userIt)
	{
		if ((*userIt)->getNickname() == oper.getNickname())
		{
			_operators.erase(userIt);
			return;
		}
	}
}

bool Channel::isOperator(const Client &user)
{
	std::vector<Client *>::iterator userIt;
	for (userIt = _operators.begin(); userIt != _operators.end(); ++userIt)
	{
		if ((*userIt)->getNickname() == user.getNickname())
			return true;
	}
	return false;
}

const std::string Channel::getName() const
{
	return this->_name;
}

void Channel::setName(const std::string &name)
{
	this->_name = name;
}

bool Channel::isBanned(const Client &user)
{
	std::vector<Client *>::iterator userIt;
	for (userIt = _bannedParticipants.begin(); userIt != _bannedParticipants.end(); ++userIt)
	{
		if ((*userIt)->getNickname() == user.getNickname())
			return true;
	}
	return false;
}

void Channel::addBan(Client &user)
{
	_bannedParticipants.push_back(&user);
}

void Channel::removeBan(const Client &user)
{
	std::vector<Client *>::iterator userIt;
	for (userIt = _bannedParticipants.begin(); userIt != _bannedParticipants.end(); ++userIt)
	{
		if ((*userIt)->getNickname() == user.getNickname())
		{
			_bannedParticipants.erase(userIt);
			return;
		}
	}
}

bool Channel::isInvited(const Client &user)
{
	std::vector<Client *>::iterator userIt;
	for (userIt = _invitedUsers.begin(); userIt != _invitedUsers.end(); ++userIt)
	{
		if ((*userIt)->getNickname() == user.getNickname())
			return true;
	}
	return false;
}

void Channel::addInviteUser(Client &user)
{
	_invitedUsers.push_back(&user);
}

void Channel::removeInviteUser(const Client &user)
{
	std::vector<Client *>::iterator userIt;
	for (userIt = _invitedUsers.begin(); userIt != _invitedUsers.end(); ++userIt)
	{
		if ((*userIt)->getNickname() == user.getNickname())
		{
			_invitedUsers.erase(userIt);
			return;
		}
	}
}

bool Channel::isAdminOnly() const
{
	return this->_adminOnlyChan;
}

void Channel::setAdminOnly()
{
	this->_adminOnlyChan = true;
}

void Channel::removeAdminOnly()
{
	this->_adminOnlyChan = false;
}

void Channel::setModes(const std::string &modes)
{
	std::string mm = _modesAdded;

	for (size_t i = 0; i < modes.length(); i++)
	{
		char mode = modes[i];
		if (_modesAdded.find(mode) == std::string::npos)
			_modesAdded += mode;
	}
}

void Channel::removeMode(const char &mode)
{
	std::string result = "";
	bool removed = false;

	for (size_t i = 0; i < _modesAdded.length(); i++)
	{
		if (_modesAdded[i] == mode && !removed)
			removed = true;
		else
			result += _modesAdded[i];
	}
	_modesAdded = result;
}

std::string Channel::getModes() const
{
	return this->_modesAdded;
}

bool Channel::isSecret() const
{
	return this->_isSecrect;
}

void Channel::setSecrect(bool to)
{
	this->_isSecrect = to;
}

void Channel::setAcceptExternalMsgs(bool to)
{
	this->_acceptExternalMsgs = to;
}

bool Channel::getAcceptExternalMsgs() const
{
	return this->_acceptExternalMsgs;
}