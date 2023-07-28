/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dangonza <dangonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 13:11:51 by dangonza          #+#    #+#             */
/*   Updated: 2023/07/27 15:49:23 by dangonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <Server.hpp>

int main(void)
{
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
