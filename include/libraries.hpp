/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libraries.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dangonza <dangonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 20:07:12 by dangonza          #+#    #+#             */
/*   Updated: 2023/07/28 15:25:34 by dangonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBRARIES_HPP
#define LIBRARIES_HPP

#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <vector>

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
