#include "LZ77.h"
#include "StreamHandler.h"
#include "Logger.h"
#include "string"
#include "BitString.h"
#include <windows.h>


int LZ77::maxWindowSize = 1024;

std::vector<LZ77Token> LZ77::getTokens(const std::vector<char>& text) {
	std::vector<LZ77Token> tokens;
	int i = 0;

	// a loop for every char to find a srting
	while (i < text.size()) {
		int maxMatchDistance = 0;
		int maxMatchLength = 0;
		int startWindow = i < maxWindowSize ? 0 : i - maxWindowSize;

		// for every char check backwards the max string
		for (int j = startWindow; j < i && maxMatchLength < i - j; j++) {
			int matchLength = 0;
			while (i + matchLength < text.size() && text[i + matchLength] == text[j + matchLength]) {
				matchLength++;
				if (j + matchLength >= i)
					break;
			}

			if (matchLength > maxMatchLength) {
				maxMatchLength = matchLength;
				maxMatchDistance = i - j;
			}
		}
		// insert the distance length & next char to the token
		char nextChar = i + maxMatchLength < text.size() ? text[i + maxMatchLength] : text[i + maxMatchLength - 1];
		maxMatchLength = i + maxMatchLength < text.size() ? maxMatchLength : maxMatchLength - 1;
		tokens.emplace_back(maxMatchDistance, maxMatchLength, nextChar);

		// Increase the index according to the length of the found string
		i += maxMatchLength + 1;
	}
	return tokens;
}

std::vector<char> LZ77::changeToString(const std::vector<LZ77Token>& tokens) {
	std::vector<char> text;
	std::string numberString;

	for (int i = 0; i < tokens.size(); i++)
	{
		// insert the offset to the vector
		numberString = std::to_string(tokens[i].offset);
		text.insert(text.end(), numberString.begin(), numberString.end());
		text.push_back('|');

		// insert the length to the vector
		numberString = std::to_string(tokens[i].length);
		text.insert(text.end(), numberString.begin(), numberString.end());
		text.push_back('|');

		// insert the char to the vector
		text.push_back(tokens[i].nextChar);
		text.push_back('|');
	}

	return text;
}

// this is fun - helper function  LZ77::decompress to find  from vector <char> a buffer charcter = |
std::string LZ77::findIndex(const std::vector<char>& vec, int& start) {
	auto it = std::find_if(vec.begin() + start, vec.end(), [](char c) {
		return c == '|';
		});
	if (it == vec.end())
		Logger::logError(Logger::NO_BUFFER_CHARACTER_FOUND);
	int help = start;
	int resIndex = static_cast<int>(it - vec.begin());
	std::string strRes = std::string(vec.begin() + help, vec.begin() + resIndex);
	start = start + static_cast<int>(strRes.length()) + 1;
	return strRes;
}

// Public function implementations
std::vector<char> LZ77::compress(std::vector<char>& text, std::unordered_map<char, BitString>& codes) {

	UNREFERENCED_PARAMETER(codes);
	Logger::logInfo(Logger::START_FUNCTION + "compress " + Logger::IN_CLASS + "LZ77");
	std::vector<LZ77Token> tokens = getTokens(text);
	std::vector<char> resultText = changeToString(tokens);
	resultText.push_back(0);
	Logger::logInfo(Logger::END_FUNCTION + " compress " + Logger::IN_CLASS + "LZ77");
	return resultText;
}

std::vector<char> LZ77::decompress(std::vector<char>& text, std::unordered_map<char, BitString>& codesMap) {


	UNREFERENCED_PARAMETER(codesMap);
	Logger::logInfo(Logger::START_FUNCTION + "decompress " + Logger::IN_CLASS + "LZ77");

	std::vector<char> decompressText;
	for (int i = 0; i < text.size() - 1; i++)
	{
		int offsetText, lengthText;

		offsetText = std::stoi(findIndex(text, i));
		lengthText = std::stoi(findIndex(text, i));
		char nextChar = ' ';
		if (i < text.size())
			nextChar = text[i++];
		int start = static_cast<int>(decompressText.size()) - offsetText;

		for (int copyText = start; copyText < lengthText + start; copyText++) {
			decompressText.push_back(decompressText[copyText]);
		}
		decompressText.push_back(nextChar);

	}
	Logger::logInfo(Logger::END_FUNCTION + " decompress " + Logger::IN_CLASS + "LZ77");
	return decompressText;
}

