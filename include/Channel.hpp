#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
#include <Client.hpp>

class Channel {
	private:
		std::vector<Client> participants;
    	std::string topic;

	public:
		Channel();
		~Channel();

		std::vector<Client> getParticipants() const;
		std::string getTopic() const;

		void setTopic(std::string newTopic);

		void addParticipant(Client participant);
		void removeParticipant(Client participant);
};

#endif
