#include "Channel.hpp"

Channel::Channel() : topic("") {}
Channel::~Channel() {}

std::vector<Client> Channel::getParticipants() const
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


void Channel::addParticipant(Client newParticipant)
{
	participants.push_back(newParticipant);
}

void Channel::removeParticipant(Client participant)
{
	std::vector<Client>::iterator userIt = std::find(participants.begin(), participants.end(), participant);
	if (userIt != participants.end())
		participants.erase(userIt);
}

bool Channel::hasParticipant(Client participant)
{
	std::vector<Client>::iterator userIt = std::find(participants.begin(), participants.end(), participant);
	if (userIt != participants.end())
		return true;
	else
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