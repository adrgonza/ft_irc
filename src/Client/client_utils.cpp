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
	if (totalParameters == 0)
		return (message);

	for (size_t i = 0; i < message.size(); i++)
	{
		if (message[i] == '<')
		{
			finalMessage += va_arg(args, char*);
			totalParameters--;
			for (size_t j = 1; (i + j) < message.size(); j++)
			{
				if (message[i + j] == '>')
				{
					i += j;
					break ;
				}
			}
			if (totalParameters <= 0) 
			{
				finalMessage += message.substr(i + 1, message.size());
				break ;
			}
		}
		else
			finalMessage += message[i];
	}
	return (finalMessage);
}
