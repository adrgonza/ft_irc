#include "Channel.hpp"

Channel::Channel(const std::vector<Client *> &clients) : _participants(clients), _topic("") {}

Channel::~Channel() {}

Channel::Channel(const Channel &obj) : _participants(obj._participants), _topic(obj._topic)
{
	this->_operators = obj._operators;
	this->_name = obj._name;
	this->_bannedParticipants = obj._bannedParticipants;
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
			return ;
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

const std::vector<Client*> Channel::getOperators() const
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
			return ;
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
	std::vector<Client>::iterator userIt = std::find(_bannedParticipants.begin(), _bannedParticipants.end(), user);
	if (userIt != _bannedParticipants.end())
		return true;
	else
		return false;
}

void Channel::addBan(const Client &user)
{
	_bannedParticipants.push_back(user);
}

void Channel::removeBan(const Client &user)
{
	std::vector<Client>::iterator userIt = std::find(_bannedParticipants.begin(), _bannedParticipants.end(), user);
	if (userIt != _bannedParticipants.end())
		_bannedParticipants.erase(userIt);
}
