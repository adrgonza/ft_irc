/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dangonza <dangonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 20:07:23 by dangonza          #+#    #+#             */
/*   Updated: 2023/07/25 17:56:49 by dangonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_HPP
#define DEFINES_HPP

#define BACKLOG 5	/* Max. client pending connections  */
#define LIMBO_CHANNEL "test" /* Channel the Server sends a Client to, when the client is not supposed to be in one (KICK, PART, etc) */
#define IRC_ENDLINE "\r\t" /* Every message sent to a client must end with this two characters */

// Todo: find a place for this prototypes
int getParameterCount(std::string errorMessage);
std::string buildClientMessage(std::string message, va_list args);
void sendMessage(int fd, std::string message, ...);

// Error messages (ready to send)
// DO NOT FORGET ENDING EACH MESSAGE WITH '\r\n'
// Please, sort them by error number, ascending order :)
#define ERR_NOSUCHCHANNEL_403		": 403 <client> <channel> :No such channel\r\n"
#define ERR_USERNOTINCHANNEL_441	": 441 <client> <nick> <channel> :They aren't on that channel\r\n"
#define ERR_NOTONCHANNEL_442		": 442 <client> <channel> :You're not on that channel\r\n"
#define ERR_NEEDMOREPARAMS_461		": 461 <client> <command> :Not enough parameters\r\n"
#define ERR_CHANOPRIVSNEEDED_482	": 482 <client> <channel> :You're not channel operator\r\n"

#endif
