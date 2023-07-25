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

int getParameterCount(std::string errorMessage)
{
	int bracketCount = 0;
	for (size_t i = 0; i < errorMessage.size(); i++)
	{
		if (errorMessage[i] == '>')
			bracketCount++;
	}
	return bracketCount;
}

std::string buildClientMessage(std::string message, va_list args)
{
	int totalParameters = getParameterCount(message);
	bool messageHasNewline = (message.size() >= 2 && message[message.size() - 2] == '\r' && message[message.size() - 1] == '\n');
	if (totalParameters == 0) // If there are no tokens in the message, do nothing about it
		return message + (messageHasNewline ? "" : IRC_ENDLINE);

	std::string finalMessage = "";
	for (size_t i = 0; i < message.size(); i++) // Loop through the string
	{
		if (message[i] == '<') // Whenever you find the start of a token (<token>)
		{
			finalMessage += va_arg(args, char*); // Adds the token to the finalMessage
			totalParameters--;
			for (size_t j = 1; (i + j) < message.size(); j++) // Skips the token on the original message
			{
				if (message[i + j] == '>')
				{
					i += j;
					break ;
				}
			}
			if (totalParameters <= 0) // If, once skipped, there are no more tokens to replace
			{
				finalMessage += message.substr(i + 1, message.size()); // Adds the rest of the message
				break ;
			}
		}
		else
			finalMessage += message[i];
	}
	return (finalMessage + (messageHasNewline ? "" : IRC_ENDLINE)); // Returns. Adds the end characters if needed
}

void sendMessage(int fd, std::string message, ...)
{
	va_list args;
	va_start(args, message);

	message = buildClientMessage(message, args);

	va_end(args);

	if (send(fd, message.c_str(), message.size(), 0) == -1)
		std::cout << "[SERVER] Error. Couldn't send message to FD " << fd << ". Message: \n\t" << message << std::cout;
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

	// Falta el port_network Y password_network
	Server myServer = Server(host, port, password);
	myServer.start();

	return (0);
}
