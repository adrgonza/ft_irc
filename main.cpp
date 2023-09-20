/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrgonza <adrgonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 13:11:51 by dangonza          #+#    #+#             */
/*   Updated: 2023/09/20 18:20:07 by adrgonza         ###   ########.fr       */
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

	Server server = Server();
	try
	{
		server.run(6667);

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

}
