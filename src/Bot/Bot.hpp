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
        Bot(std::string server, int port, std::string channel, std::string nick, std::string user, std::string password);
        ~Bot();

        void setSocket(int sock);

        std::string getServer();
        int getPort();
        std::string getChannel();
        std::string getNick();
        std::string getUserCmd();
        std::string getPassword();
        int  getSocket();
    
};

int checkArgs(int argc);
void sendConfigOpenAIMessage();
Bot sendConfigServer(char *argv[]);
std::string extractContent(const std::string &jsonResponse);
size_t WriteCallback(void *data, size_t size, size_t nmemb, std::string *response_data);
std::string isJoinCommand(std::string str, std::string channel);
std::string responseToJoinCommand(std::string buffer);


std::string sendMessageToChatbot(std::string userMessage, bool isFirstMsg);

#endif