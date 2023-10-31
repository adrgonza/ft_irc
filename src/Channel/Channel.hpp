#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <libraries.hpp>
#include "../Client/Client.hpp"

class Channel
{
private:
	std::vector<Client *> _participants;
	std::vector<Client *> _bannedParticipants;
	std::vector<Client *> _operators;
	std::vector<Client *> _invitedUsers;
	std::string _topic;
	std::string _name;
	std::string _modesAdded;
	bool _adminOnlyChan;
	bool _isSecrect;
	bool _acceptExternalMsgs;

public:
	Channel(const std::vector<Client *> &);
	Channel(const Channel &);
	~Channel();

	const std::vector<Client *> getParticipants() const;
	const std::vector<Client *> getOperators() const;
	const std::string getTopic() const;
	const std::string getName() const;

	void setTopic(const std::string &);
	void setModes(const std::string &);
	void removeMode(const char &);
	std::string getModes() const;
	void setName(const std::string &);

	void addParticipant(Client &);
	void removeParticipant(const Client &);
	void addOperator(Client &);
	void removeOperator(const Client &);

	bool isOperator(const Client &);
	bool hasParticipant(const Client &);

	void addBan(Client &);
	void removeBan(const Client &);
	bool isBanned(const Client &);

	void addInviteUser(Client &);
	void removeInviteUser(const Client &);
	bool isInvited(const Client &);

	bool isAdminOnly() const;
	void setAdminOnly();
	void removeAdminOnly();

	bool isSecret() const;
	void setSecrect(bool to);

	bool getAcceptExternalMsgs() const;
	void setAcceptExternalMsgs(bool to);
};

#endif
