#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <libraries.hpp>
#include "../Client/Client.hpp"

class Channel {
	private:
		std::vector<Client*> _participants;
		std::vector<Client> _bannedParticipants;
		std::vector<Client*> _operators;
		std::string _topic;
		std::string _name;

	public:
		Channel(const std::vector<Client*> &);
		Channel(const Channel &);
		~Channel();

		const std::vector<Client*> getParticipants() const;
		const std::vector<Client*> getOperators() const;
		const std::string getTopic() const;
		const std::string getName() const;

		void setTopic(const std::string &);
		void setName(const std::string &);

		void addParticipant(Client &);
		void removeParticipant(const Client &);
		void addOperator(Client &);
		void removeOperator(const Client &);

		bool isOperator(const Client &);

		bool hasParticipant(const Client &);

		bool isBanned(const Client &);
		void addBan(const Client &);
		void removeBan(const Client &);
};

#endif
