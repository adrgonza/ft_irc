#include <libraries.hpp>
#include "Client.hpp"

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

std::string Client::buildClientMessage(std::string message, va_list args)
{
	int totalParameters = getParameterCount(message);
	std::string finalMessage = "";
	if (totalParameters == 0) // If there are no tokens in the message, do nothing about it
		return (message);

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
	return (finalMessage);
}
