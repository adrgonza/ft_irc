#include "src/Server/Server.hpp"
#include <libraries.hpp>

bool argsManage(const std::string &port)
{
	for (size_t i = 0; i < port.length(); i++)
		if (!std::isdigit(port[i]))
			return (std::cout << "ERROR: invalid port.." << std::endl, false);
	if (std::atoi(port.c_str()) > 65535)
		return (std::cout << "\033[0;31mERROR: Invalid Port.." << std::endl, false);
	return (true);
}

int main(int argc, char **argv)
{
	if (argc != 3)
		return (std::cout << "\033[0;31mERROR: input structure expected:\033[0m ./ircserv <port> <password>" << std::endl, 1);
	std::string port = argv[1];
	std::string password = argv[2];
	if (!argsManage(port))
		return (1);
	try
	{
		Server IRC = Server(std::atoi(port.c_str()), password);

		if (IRC.run() == false)
			return (IRC.terminate_program());
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
