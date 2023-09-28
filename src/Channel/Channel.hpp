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
		std::string _name;

	public:
		Channel();
		Channel(const Channel &obj);
		~Channel();

		std::vector<Client> getParticipants() const;
		std::vector<Client> getOperators() const;
		std::string getTopic() const;
		std::string getName() const;

		void setTopic(std::string newTopic);
		void setName(std::string name);

		void addParticipant(Client participant);
		void removeParticipant(Client participant);
		void addOperator(Client oper);
		void removeOperator(Client oper);

		bool isOperator(Client user);

		bool hasParticipant(Client participant);
};

#endif
