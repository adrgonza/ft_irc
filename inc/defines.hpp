/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dangonza <dangonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 20:07:23 by dangonza          #+#    #+#             */
/*   Updated: 2023/07/25 17:04:44 by dangonza         ###   ########.fr       */
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

#endif
