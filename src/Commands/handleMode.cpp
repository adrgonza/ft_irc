#include "../Server/Server.hpp"

#include <iostream>
#include <vector>
#include <sstream>

void removeFirstWord(std::string &str)
{
	size_t pos = str.find(' ');

	if (pos != std::string::npos)
		str.erase(0, pos + 1);
	else
		str.clear();
}

void parseModes(std::string &modeString, std::vector<char> &modesToAdd, std::vector<char> &modesToRemove, std::vector<std::string> &modeParameters)
{
	std::vector<std::string> parameters;
	std::string::size_type pos = 0;
	while ((pos = modeString.find(" ")) != std::string::npos)
	{
		parameters.push_back(modeString.substr(0, pos));
		modeString = modeString.substr(pos + 1);
	}
	parameters.push_back(modeString);

	for (std::vector<std::string>::const_iterator it = parameters.begin(); it != parameters.end(); ++it)
	{
		const std::string &parameter = *it;

		if (parameter[0] == '+')
		{
			modesToAdd.push_back(parameter[1]);
			int i = 2;
			while (parameter[i] && isalpha(parameter[i]))
			{
				modesToAdd.push_back(parameter[i]);
				i++;
			}
		}
		else if (parameter[0] == '-')
		{
			modesToRemove.push_back(parameter[1]);
			int i = 2;
			while (parameter[i] && isalpha(parameter[i]))
			{
				modesToRemove.push_back(parameter[i]);
				i++;
			}
		}
		else
		{
			modeParameters.push_back(parameter);
		}
	}
}

bool notSupportedModes(std::string &body)
{
	std::string supportedModes = "Absn";

	for (size_t i = 0; i < body.length(); i++)
	{
		char modeChar = body[i];
		if (supportedModes.find(modeChar) == std::string::npos)
		{
			body.erase(i, 1);
			if (isalpha(modeChar))
				std::cout << "Erasing " << modeChar << " from param. That mode is not supported by server" << std::endl;
			i--;
		}
	}

	if (body.empty())
	{
		std::cout << "No supported modes found in the body" << std::endl;
		return (false);
	}

	return (false);
}

void Server::modeHandler(const std::string &body, const Client &user)
{
	if (body.empty())
		return;

	std::string bodyCopy = body;

	std::string channel = getWord(bodyCopy, 1);
	if (userExists(channel))
		return;

	if (!channel.empty() && channel[0] != '#')
		channel = '#' + channel;

	if (!channelExists(channel))
	{
		user.sendMessage(ERR_NOSUCHCHANNEL(user.getNickname(), channel));
		return;
	}

	Channel *chan = getChannelByName(channel);

	if (getWord(bodyCopy, 2).empty())
	{
		user.sendMessage(RPL_CHANNELMODEIS(user.getNickname(), channel, chan->getModes()));
		return;
	}

	removeFirstWord(bodyCopy);
	if (notSupportedModes((std::string &)bodyCopy))
		return;

	if (!chan->isOperator(user))
	{
		std::string nick = user.getNickname();
		user.sendMessage(ERR_CHANOPRIVSNEEDED(nick, channel));
		return;
	}

	bodyCopy = body;
	std::vector<char> modesToAdd;
	std::vector<char> modesToRemove;
	std::vector<std::string> modeParameters;
	parseModes((std::string &)bodyCopy, modesToAdd, modesToRemove, modeParameters);

	std::string modeToHandleB = getWord(body, 2);
	std::string modeToHandle = modeToHandleB;
	if (modeToHandleB.length() >= 2)
		std::string modeToHandle = modeToHandleB.substr(0, 2);

	std::string senderHost = user.getNickname() + "!user@host";

	// CHECK if !channel or user depending on mode
	std::string targetUser = getWord(body, 2);

	int modeParamI = 0;

	channel = modeParameters[modeParamI];
	modeParamI++;

	for (std::vector<char>::const_iterator it = modesToAdd.begin(); it != modesToAdd.end(); ++it)
	{
		const char &modeToHandle = *it;
		if (modeToHandle == 'A')
			adminOnlyChan(chan, senderHost, user, true);
		else if (modeToHandle == 'b')
		{
			targetUser = modeParameters[modeParamI];
			modeParamI++;
			banUser(chan, targetUser, senderHost, user);
		}
		else if (modeToHandle == 's')
			setSecretChannel(chan, senderHost, user, true);
		else if (modeToHandle == 'n')
			setExternalMsgsChannel(chan, senderHost, user, true);
	}

	for (std::vector<char>::const_iterator it = modesToRemove.begin(); it != modesToRemove.end(); ++it)
	{
		const char &modeToHandle = *it;
		if (modeToHandle == 'A')
			adminOnlyChan(chan, senderHost, user, false);
		else if (modeToHandle == 'b')
		{
			targetUser = modeParameters[modeParamI];
			modeParamI++;
			unbanUser(chan, targetUser, senderHost, user);
		}
		else if (modeToHandle == 's')
			setSecretChannel(chan, senderHost, user, false);
		else if (modeToHandle == 'n')
			setExternalMsgsChannel(chan, senderHost, user, false);
		chan->removeMode(modeToHandle);
	}
}
