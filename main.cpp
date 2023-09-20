/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrgonza <adrgonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 13:11:51 by dangonza          #+#    #+#             */
/*   Updated: 2023/09/20 18:25:15 by adrgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <Server.hpp>

int main(int argc, char **argv) {

	if (argc != 3) {
		std::cout << "\033[0;31mERROR: input structure expected:\033[0m ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	//parse port
	std::string port = argv[1];
	for (size_t i = 0; i < port.length(); i++)
		if (!std::isdigit(port[i]))
			return (std::cout << "ERROR: invalid port.." << std::endl, 1);
	if (atoi(port.c_str()) > 65535)
		return (std::cout << "\033[0;31mERROR: Invalid Port.." << std::endl, 1);

	std::string password = argv[2];
	//we have to parse password here

	Server server = Server(atoi(port.c_str()), password);

	server.run();
}
