#include <Server.hpp>
#include <iomanip>

bool checkArgs(char* argv[], std::string& host, int& port_network, std::string& password_network, int& port, std::string& password)
{
	std::string network_args(argv[1]);
	size_t pos1 = network_args.find(':');
	size_t pos2 = network_args.find(':', pos1 + 1);

	if (pos1 == std::string::npos || pos2 == std::string::npos)
	{
		std::cerr << "Invalid network arguments format. Expected format: host:port_network:password_network\n";
		return false;
	}

	host = network_args.substr(0, pos1);
	std::string port_network_str = network_args.substr(pos1 + 1, pos2 - pos1 - 1);
	std::string password_network_str = network_args.substr(pos2 + 1);

	try
	{
		port_network = std::stoi(port_network_str);
	}
	catch (const std::exception &e)
	{
		std::cerr << "Invalid network port. Expected a numeric value.\n";
		return false;
	}

	password_network = password_network_str;
	port = std::stoi(argv[2]);
	password = argv[3];

	std::cout << std::left << std::setw(15) << "Host" << "|     " << host << '\n';
    std::cout << std::setw(15) << "Network pass" << "|     " << password_network << '\n';
    std::cout << std::setw(15) << "Network port" << "|     " << port_network << '\n';
    std::cout << std::setw(15) << "Password" << "|     " << password << '\n';
    std::cout << std::setw(15) << "Port" << "|     " << port << '\n';
	return true;
}

int main(int argc, char **argv)
{
	if (argc != 4)
		return (cout_msg("Unexact number of argvs"));
    std::string host;
    int port_network;
    std::string password_network;
    int port;
    std::string password;
    if (!checkArgs(argv, host, port_network, password_network, port, password)) {
        return 1;
    }
	Server *myServer;
	// Falta el port_network Y password_network
	myServer = new Server(host, port, password);
	myServer->start();
	delete myServer;
	return (0);
}
