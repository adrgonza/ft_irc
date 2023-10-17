#include "Channel.hpp"

Channel::Channel(std::vector<Client *> clients) : participants(clients), topic("") {}

Channel::~Channel() {}

Channel::Channel(const Channel &obj) : participants(obj.participants), topic(obj.topic)
{
	this->operators = obj.operators;
	this->_name = obj._name;
	this->bannedParticipants = obj.bannedParticipants;
}

const std::vector<Client *> Channel::getParticipants() const
{
	return this->participants;
}

std::string Channel::getTopic() const
{
	return this->topic;
}

void Channel::setTopic(std::string newTopic)
{
	this->topic = newTopic;
}

void Channel::addParticipant(Client &newParticipant)
{
	participants.push_back(&newParticipant);
}

void Channel::removeParticipant(Client participant)
{
	std::vector<Client *>::iterator userIt;
	for (userIt = participants.begin(); userIt != participants.end(); ++userIt)
	{
		if ((*userIt)->getNickname() == participant.getNickname())
		{
			participants.erase(userIt);
			return ;
		}
	}
}

bool Channel::hasParticipant(Client participant)
{
	std::vector<Client *>::iterator userIt;
	for (userIt = participants.begin(); userIt != participants.end(); ++userIt)
	{
		if ((*userIt)->getNickname() == participant.getNickname())
			return true;
	}
	return false;
}

std::vector<Client> Channel::getOperators() const
{
	return this->operators;
}

void Channel::addOperator(Client newOper)
{
	operators.push_back(newOper);
}

void Channel::removeOperator(Client oper)
{
	std::vector<Client>::iterator userIt = std::find(operators.begin(), operators.end(), oper);
	if (userIt != operators.end())
		operators.erase(userIt);
}

bool Channel::isOperator(Client user)
{
	std::vector<Client>::iterator userIt = std::find(operators.begin(), operators.end(), user);
	if (userIt != operators.end())
		return true;
	else
		return false;
}

std::string Channel::getName() const
{
	return this->_name;
}

void Channel::setName(std::string name)
{
	this->_name = name;
}

bool Channel::isBanned(Client user)
{
	std::vector<Client>::iterator userIt = std::find(bannedParticipants.begin(), bannedParticipants.end(), user);
	if (userIt != bannedParticipants.end())
		return true;
	else
		return false;
}

void Channel::addBan(Client user)
{
	bannedParticipants.push_back(user);
}

void Channel::removeBan(Client user)
{
	std::vector<Client>::iterator userIt = std::find(bannedParticipants.begin(), bannedParticipants.end(), user);
	if (userIt != bannedParticipants.end())
		bannedParticipants.erase(userIt);
}