#include "Bot.hpp"

std::string firstMessageForSetup = "{"
		"\"model\": \"gpt-3.5-turbo\","
		"\"messages\": [{\"role\": \"system\", \"content\": \"Comportate como un bot de chat, tu nombre es Juan, tienes 20 años y te gustan los deportes\"}],"
		"\"temperature\": 0.5,"
		"\"max_tokens\": 256"
"}";

int checkArgs(const int &argc)
{
	if (argc != 3)
	{
		std::cout << "Parameters expected: ./ircbot (server_password) (channel_to_join)" << std::endl;
		return -1;
	}
	return 1;
}

void sendConfigOpenAIMessage()
{
	std::string response = sendMessageToChatbot(firstMessageForSetup.c_str(), true);
	response = extractContent(response);
	std::cout << "response: " << response << std::endl;
}

Bot sendConfigServer(char *argv[])
{
	std::string channel = argv[2];
	if (!channel.empty() && channel[0] != '#')
		channel = "#" + channel;
	Bot newBot("127.0.0.1", 6667, channel, "bot", "bot 0 * :bot", argv[1]);

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	newBot.setSocket(sock);
	if (sock == -1)
	{
		std::cerr << "Error creating socket" << std::endl;
		return newBot;
	}
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(newBot.getPort());
	serverAddr.sin_addr.s_addr = inet_addr(newBot.getServer().c_str());
	if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
	{
		newBot.setSocket(-1);
		std::cerr << "Error connecting to server" << std::endl;
		return newBot;
	}
	std::string message;
	usleep(500000);
	message = "PASS " + newBot.getPassword() + "\r\n";
	send(sock, message.c_str(), message.length(), 0);
	usleep(500000);
	message = "USER " + newBot.getUserCmd() + "\r\n";
	send(sock, message.c_str(), message.length(), 0);
	usleep(500000);
	message = "NICK " + newBot.getNick() + "\r\n";
	send(sock, message.c_str(), message.length(), 0);
	usleep(1000000);
	message = "JOIN " + newBot.getChannel() + "\r\n";
	send(sock, message.c_str(), message.length(), 0);
	std::cout << "Information sent to the server" << std::endl;
	return newBot;
}

std::string extractContent(const std::string &jsonResponse)
{
	std::string contentKey = "\"content\": \"";
	size_t contentStart = jsonResponse.find(contentKey);

	if (contentStart == std::string::npos)
	{
		return "Content not found.";
	}

	contentStart += contentKey.length();

	size_t contentEnd = jsonResponse.find("\"", contentStart);

	if (contentEnd == std::string::npos)
	{
		return "Invalid JSON format.";
	}

	std::string content = jsonResponse.substr(contentStart, contentEnd - contentStart);
	return content;
}

size_t WriteCallback(void *data, size_t size, size_t nmemb, std::string *response_data)
{
    size_t total_size = size * nmemb;
    response_data->append(static_cast<char *>(data), total_size);
    return total_size;
}

bool endsWith(const std::string &fullString, const std::string &channel) {
	std::string expectedEnding = "JOIN " + channel;

	std::string modifiedString = fullString;

	std::string::size_type newlinePos = modifiedString.find('\n');
	if (newlinePos != std::string::npos) {
		modifiedString.erase(newlinePos);
	}

	std::cout << "comparing: " << modifiedString << " = " << expectedEnding << "." << std::endl;

	if (modifiedString.length() >= expectedEnding.length() &&
		std::strcmp(modifiedString.c_str() + modifiedString.length() - expectedEnding.length(), expectedEnding.c_str()) == 0) {
		return true;
	}
	return false;
}

std::string isJoinCommand(const std::string &input, const std::string &channel)
{
	std::string response;
	if (endsWith(input, channel))
	{
		std::cout << "inside" << std::endl;
		std::string::size_type pos1 = input.find('!');
		std::string::size_type pos2 = input.find('@');

		if (pos1 != std::string::npos && pos2 != std::string::npos) {
			std::cout << "-inside" << std::endl;
			std::string name = input.substr(1, pos1 - 1);
			std::string sameName = input.substr(pos1 + 2, pos2 - pos1 - 2);
			std::string localhost = input.substr(pos2 + 1);
			std::cout << "name, sameName, localhost" << std::endl;
			std::cout << name << sameName << localhost << std::endl;
			return name;
		}
	}
	return response;
}

std::string responseToJoinCommand(std::string buffer)
{
	buffer = "";
	return ("Helloooooooo o o");
}
