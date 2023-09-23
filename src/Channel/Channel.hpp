#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
#include "../src/Client/Client.hpp"

class Channel {
	private:
		std::vector<Client> participants;
		std::vector<Client> operators;
    	std::string topic;

	public:
		Channel();
		~Channel();

		std::vector<Client> getParticipants() const;
		std::vector<Client> getOperators() const;
		std::string getTopic() const;

		void setTopic(std::string newTopic);

		void addParticipant(Client participant);
		void removeParticipant(Client participant);
		void addOperator(Client oper);
		void removeOperator(Client oper);

		bool isOperator(Client user);
};

#endif
