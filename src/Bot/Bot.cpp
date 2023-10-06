#include <curl/curl.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <libraries.hpp>

std::string apiKey = "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";

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

std::string sendMessageToChatbot(std::string userMessage) {
	CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL." << std::endl;
        return "";
    }

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + std::string(apiKey)).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    std::string response_data;

    // Construct a JSON request object
    std::string json_request = "{"
        "\"model\": \"gpt-3.5-turbo\","
        "\"messages\": [{\"role\": \"system\", \"content\": \"You: " + userMessage + "\"}],"
        "\"temperature\": 0.5,"
        "\"max_tokens\": 256"
    "}";

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_request.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, json_request.size());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "CURL request failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        return "";
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

	long http_response_code;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);

    return response_data;
}

void analyzeMessage(const std::string &buffer, int sock)
{
	const std::string NICK = "bot";
	size_t firstSpace = buffer.find(' ');
	if (firstSpace != std::string::npos)
	{
		std::string username = buffer.substr(1, firstSpace - 1);

		size_t hash_pos = buffer.find('#', firstSpace);
		if (hash_pos != std::string::npos)
		{
			size_t secondSpace = buffer.find(' ', hash_pos);
			if (secondSpace != std::string::npos)
			{
				std::string chan = buffer.substr(hash_pos, secondSpace - hash_pos);
				std::string analyzed_message = buffer.substr(secondSpace + 1);
				std::string cleaned_message;
				for (std::string::iterator it = analyzed_message.begin(); it != analyzed_message.end(); ++it)
				{
					char c = *it;
					if (c != '\r' && c != '\n')
						cleaned_message += c;
					if (c == '/' || c == '@' || (c >= '0' && c <= '9'))
						return;
				}
				std::cout << "Username: " << username << std::endl;
				std::cout << "Channel: " << chan << std::endl;
				std::cout << "Message: " << cleaned_message << std::endl;
				std::string response = sendMessageToChatbot(cleaned_message.c_str());
				response = extractContent(response);
				std::string msgResponse = "PRIVMSG " + chan + " :" + std::string(username) + " " + response + IRC_ENDLINE;
				std::cout << "sending: " << msgResponse << std::endl;
				send(sock, msgResponse.c_str(), msgResponse.length(), 0);
			}
		}
	}
}

int main()
{
	const std::string SERVER = "127.0.0.1";
	const int PORT = 6667;
	const std::string CHANNEL = "#pepe";
	const std::string NICK = "bot";
	const std::string USER = "bot 0 * :bot";
	const std::string PASSWORD = "hola";
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		std::cerr << "Error creating socket" << std::endl;
		return 1;
	}
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER.c_str());
	if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
	{
		std::cerr << "Error connecting to server" << std::endl;
		return 1;
	}
	std::string buffer(4096, '\0');
	std::string message;
	usleep(500000);
	message = "PASS " + PASSWORD + "\r\n";
	send(sock, message.c_str(), message.length(), 0);
	usleep(500000);
	message = "USER " + USER + "\r\n";
	send(sock, message.c_str(), message.length(), 0);
	usleep(500000);
	message = "NICK " + NICK + "\r\n";
	send(sock, message.c_str(), message.length(), 0);
	usleep(1000000);
	message = "JOIN " + CHANNEL + "\r\n";
	send(sock, message.c_str(), message.length(), 0);
	std::cout << "Information sent to the server" << std::endl;
	while (1)
	{
		memset(&buffer[0], 0, buffer.size());
		int bytesReceived = recv(sock, &buffer[0], buffer.size(), 0);
		if (bytesReceived == -1)
		{
			std::cerr << "Error receiving data from server" << std::endl;
			break;
		}
		if (bytesReceived == 0)
		{
			std::cerr << "The server closed the connection" << std::endl;
			break;
		}
		analyzeMessage(buffer, sock);
	}
	close(sock);
	return 0;
}