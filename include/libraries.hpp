#ifndef LIBRARIES_HPP
#define LIBRARIES_HPP

// c++
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdarg>
#include <vector>
#include <map>


// c
#include <strings.h>
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <cerrno>
#include <sys/socket.h>
#include <limits.h>
#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>

typedef enum
{
	CMD_UNKNOWN,
	CMD_NICK,
	CMD_USER,
	CMD_PING,
	CMD_PONG,
	CMD_QUIT,
	CMD_JOIN,
	CMD_PART,
	CMD_TOPIC,
	CMD_NAMES,
	CMD_LIST,
	CMD_KICK,
	CMD_PRIVMSG,
	CMD_NOTICE,
	CMD_SAY,
	CMD_PASS,
	CMD_INVITE,
	CMD_MODE,
	CMD_WHO,
	CMD_WHOIS,
	CMD_WHOWAS,
	CMD_BAN,
	CMD_UNBAN
} e_command;

e_command parseCommandCode(std::string command);

#define IRC_ENDLINE "\r\n"

// Error Messages
#define ERR_UNKNOWNCOMMAND_421(client, command) ("421 " + (client) + "  " + (command) + " :Unknown command")
#define ERR_NOSUCHNICK(client, nickname) ("401 " + (client) + " " + (nickname) + " :No such nick/channel")
#define ERR_NOSUCHCHANNEL(client, channel) ("403 " + (client) + " " + (channel) + " :No such channel")
#define ERR_NOCHANNELS(client) ("403 " + (client) + " :There are no channels")
#define ERR_CANNOTSENDTOCHAN(client, channel) ("404 " + (client) + " " + (channel) + " :Cannot send to channel")
#define ERR_TOOMANYCHANNELS(client, channel) ("405 " + (client) + " " + (channel) + " :You have joined too many channels")
#define ERR_WASNOSUCHNICK(client) ("406 " + (client) + " :There was no such nickname")
#define ERR_NORECIPIENT(client, command) ("411 " + (client) + " :No recipient given (" + (command) + ")")
#define ERR_NOTEXTTOSEND(client) ("412 " + (client) + " :No text to send")
#define ERR_NONICKNAMEGIVEN(client) ("431 " + (client) + " :No nickname given")
#define ERR_NICKNAMEINUSE(client, nick) ("433 " + (client) + " " + (nick) + " :Nickname is already in use")
#define ERR_NOTONCHANNEL(client, channel) ("442 " + (client) + " " + (channel) + " :You're not on that channel")
#define ERR_USERONCHANNEL(client, nick, channel) ("443 " + (client) + " " + (nick) + " " + (channel) + " :is already on channel")
#define ERR_CHANOPRIVSNEEDED(client, channel) ("482 " + (client) + " " + (channel) + " :You're not channel operator")
#define ERR_PASSWDMISMATCH(client) ("464 " + (client) + " :Password incorrect")
#define ERR_PASSWDREQUIRED(client) ("421 " + (client) + " :Password required")
#define ERR_YOUREBANNEDCREEP(client) ("465 " + (client) + " :You are banned from this server.")
#define ERR_BANNEDFROMCHAN(client, channel) ("474 " + (client) + " " + (channel) + " :Cannot join channel (+b)")
#define ERR_USERALREADYBANNED(nickname, channel) ("484 " + (nickname) + " " + (channel) + " :Cannot ban user; they are already banned (+b)")

// Command Messages
#define USER_CMD(username, realname) ("USER " + (username) + " 0 * " + (realname))
#define NICK_CMD(nickname) ("NICK " + (nickname))
#define PRIVMSG_CMD(nickname, target, text) (":" + (nickname) + " PRIVMSG " + (target) + " " + (text))
#define NOTICE_CMD(nickname, target, text) (":" + (nickname) + " NOTICE " + (target) + " " + (text))
#define NOTICE_RECEIVER_CMD(sender, target, text) (":" + (sender) + " NOTICE " + (target) + " " + (text))
#define PRIVMSG_RECEIVER_CMD(sender, target, text) (":" + (sender) + " PRIVMSG " + (target) + " " + (text))
#define JOIN_CMD(channel) ("JOIN " + (channel))
#define PART_CMD(nickname, channel) (":" + (nickname) + "!user@host PART " + (channel))
#define TOPIC_CMD(channel, topic) ("TOPIC " + (channel) + " " + (topic))
#define NAMES_CMD(channel) ("NAMES " + (channel))
#define LIST_CMD "LIST"
#define INVITE_CMD(nickname, targetNick, channel) (":" + (nickname) + "!user@host INVITE " + (targetNick) + " " + (channel))
#define KICK_CMD(user, channel, target) (":" + user + "KICK " + (channel) + " " + (target))
#define QUIT_CMD(server, reason) (":" + (server) + " QUIT :" + (reason))
#define RPL_MOTD(server, client, text) (":" + (server) + " 372 " + (client) + " :" + (text))
#define RPL_MOTDSTART(client, text) ("375 " + (client) + " : " + (text))
#define RPL_ENDOFMOTD(server, client) (":" + (server) + " 376 " + (client) + " :End of welcome msg")
#define RPL_WHOREPLY(server, client, channel, username, host, serverName, nick, flags, hopcount, realname) \
    (":" + (server) + " 352 " + (client) + " " + (channel) + " " + (username) + " " + (host) + " " + (serverName) + " " + (nick) + " " + (flags) + " :" + (hopcount) + " " + (realname))
#define RPL_ENDOFWHO(server, client, mask) (":" + (server) + " 315 " + (client) + " " + (mask) + " :End of WHO list")
#define RPL_WHOISUSER(server, client, nick, username, host, realname) \
    (":" + (server) + " 311 " + (client) + " " + (nick) + " " + (username) + " " + (host) + " * :" + (realname))
#define RPL_ENDOFWHOIS(server, client, nick) (":" + (server) + " 318 " + (client) + " " + (nick) + " :End of /WHOIS list")
#define RPL_WHOWASUSER(server, client, nick, username, host, realname) \
    (":" + (server) + " 319 " + (client) + " " + (nick) + " " + (username) + " " + (host) + " * :" + (realname))
#define RPL_ENDOFWHOWAS(server, client, nick) (":" + (server) + " 319 " + (client) + " " + (nick) + " :End of WHOWAS")
#define PING_CMD(token) ("PING " + (token))
#define PONG_CMD(token) ("PONG :" + (token))
#define OPER_CMD(name, password) ("OPER " + (name) + " " + (password))
#define BAN_CMD(server, channel, host) (":" + (server) + " MODE " + (channel) + " +b " + (host))
#define UNBAN_CMD(server, channel, host) (":" + (server) + " MODE " + (channel) + " -b " + (host))



#endif
