/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antgonza <antgonza@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 15:51:47 by antgonza          #+#    #+#             */
/*   Updated: 2023/09/27 20:31:51 by antgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server/Server.hpp"

void Server::noticeMessage(std::string body, Client &user)
{
    std::istringstream iss(body);
    std::string target;
    std::string message;

    iss >> target;
    std::getline(iss, message);
    if (!message.empty() && message[0] == ' ')
        message = message.substr(1);
    if (!message.empty() && message[0] == ':')
        message = message.substr(1);
    std::cout << "Primera Palabra-" << target << "-" << std::endl;
    std::cout << "Resto del String-" << message << "-" << std::endl;
    return;
}

/* ************************************************************************** */
