#include <Channel.hpp>

Channel::Channel() : topic("") {}
Channel::~Channel() {}

std::vector<std::string> Channel::getParticipants() const
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


void Channel::addParticipant(std::string newParticipant)
{
	participants.push_back(newParticipant);
}

void Channel::removeParticipant(std::string participant)
{
	std::vector<std::string>::iterator userIt = std::find(participants.begin(), participants.end(), participant);
    if (userIt != participants.end())
        participants.erase(userIt);
}

