#ifndef LIBRARIES_HPP
#define LIBRARIES_HPP

//c++
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

//c
#include <unistd.h>
#include <poll.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>

typedef enum {
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
	CMD_SQUIT
} e_command;

e_command parseCommandCode(std::string command);

// Error messages
#define IRC_ENDLINE "\r\n"
#define ERR_UNKNOWNCOMMAND_421 "421 <client> <command> :Unknown command"

#endif
