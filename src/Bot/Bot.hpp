#ifndef BOT_HPP
# define BOT_HPP

#include <curl/curl.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define OPENAI_API_KEY "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"

class Bot {
	private:
		const std::string _server;
		const int _port;
		std::string _channel;
		std::string _nick;
		std::string _userCmd;
		std::string _password;
		int _sock;

	public:
		Bot(const std::string &, const int &, const std::string &, const std::string &, const std::string &, const std::string &);
		~Bot();

		void setSocket(const int &);

		std::string getServer();
		int getPort();
		std::string getChannel();
		std::string getNick();
		std::string getUserCmd();
		std::string getPassword();
		int  getSocket();

};

int checkArgs(const int &);
void sendConfigOpenAIMessage();
Bot sendConfigServer(char *argv[]);
std::string extractContent(const std::string &);
size_t WriteCallback(void *, const size_t &, const size_t &, std::string *response_data);
std::string isJoinCommand(const std::string &, const std::string &);
std::string responseToJoinCommand(std::string buffer);


std::string sendMessageToChatbot(std::string userMessage, bool isFirstMsg);

#endif
