#include <libraries.hpp>
#include "Bot.hpp"

std::string sendMessageToChatbot(std::string userMessage, bool isFirstMsg) {
	CURL* curl = curl_easy_init();
	if (!curl) {
		std::cerr << "Failed to initialize CURL." << std::endl;
		return "";
	}

	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, ("Authorization: Bearer " + std::string(OPENAI_API_KEY)).c_str());
	headers = curl_slist_append(headers, "Content-Type: application/json");

	std::string response_data;

	// Construct a JSON request object
	std::string json_request = "{"
		"\"model\": \"gpt-3.5-turbo\","
		"\"messages\": [{\"role\": \"user\", \"content\": \"You: " + userMessage + "\"}],"
		"\"temperature\": 0.5,"
		"\"max_tokens\": 256"
	"}";

	if (isFirstMsg)
		json_request = userMessage;

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

void analyzeMessage(const std::string &buffer, Bot *myBot)
{
	std::cout << "buf: " << std::endl << "-" << buffer << "-" << std::endl;
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
				std::string response = sendMessageToChatbot(cleaned_message.c_str(), false);
				std::cout << "response: " << response << std::endl;
				response = extractContent(response);
				std::string msgResponse = "PRIVMSG " + chan + " :" + std::string(username) + " " + response + IRC_ENDLINE;
				std::cout << "sending: " << msgResponse << std::endl;
				send(myBot->getSocket(), msgResponse.c_str(), msgResponse.length(), 0);
			}
		}
	}
}

int main(int argc, char *argv[])
{
	if (checkArgs(argc) == -1)
		return 1;
	Bot newBot = sendConfigServer(argv);
	if (newBot.getSocket() == -1)
		return 2;
	sendConfigOpenAIMessage();
	std::string buffer(4096, '\0');
	while (1)
	{
		memset(&buffer[0], 0, buffer.size());
		int bytesReceived = recv(newBot.getSocket(), &buffer[0], buffer.size(), 0);
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
		analyzeMessage(buffer, &newBot);
	}
	close(newBot.getSocket());
	return 0;
}
