#include <Server.hpp>

int cout_msg(std::string msg)
{
	std::cout << msg << std::endl;
	return (0);
}

void sendMsgToClient(std::string msg, int clientFd)
{
	int retValue = send(clientFd, msg.c_str(), msg.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
}

void sendErrorMsgToClient(std::string msg, int errorCode, std::string nickname, int clientFd, std::string optParam)
{
	std::string errorMessage;
	if (optParam.empty())
		errorMessage = ": " + std::to_string(errorCode) + " " + nickname + " :" + msg + "\r\n";
	else
		errorMessage = ": " + std::to_string(errorCode) + " " + nickname + " :" + optParam + " - " + msg + "\r\n";
	int retValue = send(clientFd, errorMessage.c_str(), errorMessage.size(), 0);
	if (retValue == -1)
		std::cerr << "[SERVER-error]: send failed " << errno << strerror(errno) << std::endl;
	return;
}
