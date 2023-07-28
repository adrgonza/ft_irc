/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dangonza <dangonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 20:38:14 by dangonza          #+#    #+#             */
/*   Updated: 2023/07/28 15:38:15 by dangonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libraries.hpp>
#include <Client.hpp>

e_command parseCommandCode(std::string command)
{
	if (command == "NICK") return CMD_NICK;
	if (command == "USER") return CMD_USER;
	if (command == "PING") return CMD_PING;
	if (command == "PONG") return CMD_PONG;
	if (command == "OPER") return CMD_OPER;
	if (command == "AUTHENTICATE") return CMD_AUTH;
	if (command == "QUIT") return CMD_QUIT;
	if (command == "JOIN") return CMD_JOIN;
	if (command == "PART") return CMD_PART;
	if (command == "TOPIC") return CMD_TOPIC;
	if (command == "NAMES") return CMD_NAMES;
	if (command == "LIST") return CMD_LIST;
	if (command == "KICK") return CMD_KICK;
	if (command == "PRIVMSG") return CMD_PRIVMSG;
	if (command == "NOTICE") return CMD_NOTICE;
	if (command == "KILL") return CMD_KILL;
	if (command == "ERROR") return CMD_ERROR;
	if (command == "PASS") return CMD_PASS;
	if (command == "CAP") return CMD_CAP;
	if (command == "INVITE") return CMD_INVITE;
	if (command == "TIME") return CMD_TIME;
	if (command == "MODE") return CMD_MODE;
	if (command == "WHO") return CMD_WHO;
	if (command == "WHOIS") return CMD_WHOIS;
	if (command == "WHOWAS") return CMD_WHOWAS;
	if (command == "REHASH") return CMD_REHASH;
	if (command == "RESTART") return CMD_RESTART;
	if (command == "SQUIT") return CMD_SQUIT;
	return CMD_UNKNOWN;
}

void handleCommand(Client caller, std::string command, std::string body)
{
	e_command commandCode = parseCommandCode(command);
	switch (commandCode)
	{
		// Commands done
		case CMD_NICK: caller.doNickCommand(body); break;

		// Commands yet to do
		case CMD_USER:
		case CMD_PING:
		case CMD_PONG:
		case CMD_OPER:
		case CMD_AUTH:
		case CMD_QUIT:
		case CMD_JOIN:
		case CMD_PART:
		case CMD_TOPIC:
		case CMD_NAMES:
		case CMD_LIST:
		case CMD_KICK:
		case CMD_PRIVMSG:
		case CMD_NOTICE:
		case CMD_KILL:

		// Not sure if needed
		case CMD_ERROR:
		case CMD_PASS:
		case CMD_CAP:
		case CMD_INVITE:
		case CMD_TIME:
		case CMD_MODE:
		case CMD_WHO:
		case CMD_WHOIS:
		case CMD_WHOWAS:
		case CMD_REHASH:
		case CMD_RESTART:
		case CMD_SQUIT:
		case CMD_UNKNOWN:
			caller.sendMessage(ERR_UNKNOWNCOMMAND_421, caller.getNickname().c_str(), command.c_str());
		break;
	}
}
