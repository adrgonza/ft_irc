#include <libraries.hpp>
#include "Client.hpp"

std::string Client::getWord(const std::string &str, int wordNumber)
{
	if (str.empty())
		return "";

	std::string::size_type startPos = 0;
	std::string::size_type endPos = 0;
	int currentWord = 0;

	while (currentWord < wordNumber)
	{
		startPos = str.find_first_not_of(' ', endPos);
		if (startPos == std::string::npos)
			return "";
		endPos = str.find(' ', startPos);
		if (endPos == std::string::npos)
			endPos = str.length();
		currentWord++;
	}

	return str.substr(startPos, endPos - startPos);
}