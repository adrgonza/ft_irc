#ifndef LIBRARIES_HPP
#define LIBRARIES_HPP

// c++
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

// c
#include <unistd.h>
#include <poll.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>

typedef enum
{
	CMD_UNKNOWN, // Unknown command
	CMD_NICK,
	CMD_USER,
	CMD_PING,
	CMD_PONG,
	CMD_OPER,
	CMD_AUTH,
	CMD_QUIT,
	CMD_JOIN,
	CMD_PART,
	CMD_TOPIC,
	CMD_NAMES,
	CMD_LIST,
	CMD_KICK,
	CMD_PRIVMSG,
	CMD_NOTICE,
	CMD_KILL,
	CMD_SAY,
	// I'm not sure if the commands below are needed
	CMD_ERROR,
	CMD_PASS,
	CMD_CAP,
	CMD_INVITE,
	CMD_TIME,
	CMD_MODE,
	CMD_WHO,
	CMD_WHOIS,
	CMD_WHOWAS,
	CMD_REHASH,
	CMD_RESTART,
	CMD_SQUIT,
	CMD_BAN,
	CMD_UNBAN
} e_command;

e_command parseCommandCode(std::string command);

#define IRC_ENDLINE "\r\n"

// Error Messages
#define ERR_UNKNOWNCOMMAND_421 "421 <client> <command> :Unknown command"
#define ERR_NOSUCHNICK "401 <client> <nickname> :No such nick/channel"
#define ERR_NOSUCHCHANNEL "403 <client> <channel> :No such channel"
#define ERR_NOCHANNELS "403 <client> :There are no channels"
#define ERR_CANNOTSENDTOCHAN "404 <client> <channel> :Cannot send to channel"
#define ERR_TOOMANYCHANNELS "405 <client> <channel> :You have joined too many channels"
#define ERR_WASNOSUCHNICK "406 <client> :There was no such nickname"
#define ERR_NORECIPIENT "411 <client> :No recipient given (<command>)"
#define ERR_NOTEXTTOSEND "412 <client> :No text to send"
#define ERR_NONICKNAMEGIVEN "431 <client> :No nickname given"
#define ERR_NICKNAMEINUSE "433 <client> <nick> :Nickname is already in use"
#define ERR_NOTONCHANNEL "442 <client> <channel> :You're not on that channel"
#define ERR_USERONCHANNEL "443 <client> <nick> <channel> :is already on channel"
#define ERR_CHANOPRIVSNEEDED "482 <client> <channel> :You're not channel operator"
#define ERR_PASSWDMISMATCH "464 <client> :Password incorrect"
#define ERR_PASSWDREQUIRED "421 <client> :Password required"
#define ERR_YOUREBANNEDCREEP "465 <client> :You are banned from this server."
#define ERR_BANNEDFROMCHAN "474 <client> <channel> :Cannot join channel (+b)"
#define ERR_USERALREADYBANNED "484 <nickname> <channel> :Cannot ban user; they are already banned (+b)"


// Command Messages
#define USER_CMD "USER <username> 0 * <realname>"
#define NICK_CMD "NICK <nickname>"
#define PRIVMSG_CMD "PRIVMSG <target> <text>" // PRIVMSG Parameters: <target>{,<target>} <text to be sent>
#define NOTICE_CMD "NOTICE <target> <text>"
#define NOTICE_RECEIVER_CMD ":<sender> NOTICE <target> <text>"
#define PRIVMSG_RECEIVER_CMD ":<sender> PRIVMSG <target> <text>" // PRIVMSG Parameters: <target>{,<target>} <text to be sent>
#define JOIN_CMD "JOIN <channel>"								 // <channel>{,<channel>} [<key>{,<key>}]
#define PART_CMD ":<nickname>!user@host PART <channel>"			 // <channel>{,<channel>} [<reason>]
#define TOPIC_CMD "TOPIC <channel> <topic>"
#define NAMES_CMD "NAMES <channel>" // <channel>{,<channel>}
#define LIST_CMD "LIST"				// [<channel>{,<channel>}] [<elistcond>{,<elistcond>}]
#define INVITE_CMD ":<nickname>!user@host INVITE <targetNick> <channel>"
#define KICK_CMD "KICK <channel> <user>" //<channel> <user> *( "," <user> ) [<comment>]
#define MOTD "<client> - <server> -"
#define RPL_WHOREPLY ":<server> 352 <client> <channel> <username> <host> <server> <nick> <flags> :<hopcount> <realname>"
#define RPL_ENDOFWHO ":<server> 315 <client> <mask> :End of WHO list"
#define RPL_WHOISUSER ":<server> 311 <client> <nick> <username> <host> * :<realname>"
#define RPL_ENDOFWHOIS ":<server> 318 <client> <nick> :End of /WHOIS list"
#define RPL_WHOWASUSER ":<server> 319 <client> <nick> <username> <host> * :<realname>"
#define RPL_ENDOFWHOWAS ":<server> 319 <client> <nick> :End of WHOWAS"
#define PING_CMD "PING <token>"
#define OPER_CMD "OPER <name> <password>"
#define BAN_CMD ":<server> MODE <channel> +b <host>"
#define UNBAN_CMD ":<server> MODE <channel> -b <host>"

#endif
