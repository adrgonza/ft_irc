#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <libraries.hpp>
#include "../Client/Client.hpp"

class Channel {
	private:
		std::vector<Client*> participants;
		std::vector<Client> bannedParticipants;
		std::vector<Client> operators;
		std::string topic;
		std::string _name;

	public:
		Channel(std::vector<Client*> clients);
		Channel(const Channel &obj);
		~Channel();

		const std::vector<Client*> getParticipants() const;
		std::vector<Client> getOperators() const;
		std::string getTopic() const;
		std::string getName() const;

		void setTopic(std::string newTopic);
		void setName(std::string name);

		void addParticipant(Client& participant);
		void removeParticipant(Client participant);
		void addOperator(Client oper);
		void removeOperator(Client oper);

		bool isOperator(Client user);

		bool hasParticipant(Client participant);

		bool isBanned(Client user);
		void addBan(Client user);
		void removeBan(Client user);
};

#endif
