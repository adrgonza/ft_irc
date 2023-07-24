#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>

class Channel {
	private:
		std::vector<std::string> participants;
    	std::string topic;

	public:
		Channel();
		~Channel();

		std::vector<std::string> getParticipants() const;
		std::string getTopic() const;

		void setTopic(std::string newTopic);

		void addParticipant(std::string newParticipant);
		void removeParticipant(std::string participant);
};

#endif
