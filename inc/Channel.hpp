#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
#include "defines.hpp"

class Channel {
	private:
		std::vector<std::string> participants;
		std::vector<std::string> operators;
    	std::string topic;

	public:
		Channel();
		~Channel();

		std::vector<std::string> getParticipants() const;
		std::vector<std::string> getOperators() const;
		std::string getTopic() const;

		void setTopic(std::string newTopic);

		void addParticipant(std::string newParticipant);
		void removeParticipant(std::string participant);

		void addOperator(std::string newOperator);
		void removeOperator(std::string operatorName);

		bool hasUser(std::string participant);
		bool hasOperator(std::string operatorName);
};

#endif
