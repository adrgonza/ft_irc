#include "../server/Server.hpp"

std::string commands[4] = {"JOIN", "INVITE", "LIST", "ME"};

void welcomeClient() {

}

void doIrcCommand(std::string buffer) {
	std::string command = "";
	std::string param;
	bool possibleCommand = false;

	int i = -1;
	while (buffer[++i] && buffer[i] != ' ')
		command += buffer[i];
	for (int i = 0; i < 4; i++) {
		if (command == commands[i]) {
			possibleCommand = true;
			break;
		}
	}
	if (possibleCommand) {
		std::cout << "command found: " << command << std::endl;
	} else
		std::cout << "command NOT found" << std::endl;
}