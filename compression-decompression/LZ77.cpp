#include "LZ77.h"
#include "StreamHandler.h"
#include "Logger.h"
#include "string"

namespace LZ77 {

	int maxWindowSize = 1024; 

	// Internal function implementations
	namespace Detail {

		// get a vector of char and return a vector of LZ77Token struct that containing:
		// the strings with length, offset and next char 
		

		// get tokens - a vector of LZ77Token struct and return a vector of char that containing:
		// all the value from the tokens with the character '|' between each character
		std::vector<char> changeToString(const std::vector<LZ77Token>& tokens) {
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
		std::string findIndex(const std::vector<char>& vec, int& start) {
			auto it = std::find_if(vec.begin() + start, vec.end(), [](char c) {
				return c == '|';
				});
			if (it == vec.end())
				Logger::logError(Logger::NO_BUFFER_CHARACTER_FOUND);
			int help = start;
			int resIndex = it - vec.begin();
			std::string strRes = std::string(vec.begin() + help, vec.begin() + resIndex);
			start = start + strRes.length() + 1;
			return strRes;
		}
	}

	// Public function implementations
	std::vector<char> compress(std::vector<char>& text, std::unordered_map<char, std::string>& codes) {
		Logger::logInfo(Logger::START_FUNCTION + "compress " + Logger::IN_CLASS + "LZ77");
		std::vector<LZ77Token> tokens = Detail::getTokens(text);
		std::vector<char> resultText = Detail::changeToString(tokens);
		resultText.push_back(0);
		Logger::logInfo(Logger::END_FUNCTION + " compress " + Logger::IN_CLASS + "LZ77");
		return resultText;
	}

	std::vector<char> decompress(std::vector<char>& text, std::unordered_map<char, std::string>& codesMap) {
		Logger::logInfo(Logger::START_FUNCTION + "decompress " + Logger::IN_CLASS + "LZ77");

		std::vector<char> decompressText;
		for (int i = 0; i < text.size() - 1; i++)
		{
			int offsetText, lengthText;

			offsetText = std::stoi(Detail::findIndex(text, i));
			lengthText = std::stoi(Detail::findIndex(text, i));
			char nextChar = ' ';
			if (i < text.size())
				nextChar = text[i++];
			int start = decompressText.size() - offsetText;

			for (int copyText = start; copyText < lengthText + start; copyText++) {
				decompressText.push_back(decompressText[copyText]);
			}
			decompressText.push_back(nextChar);

		}
		Logger::logInfo(Logger::END_FUNCTION + " decompress " + Logger::IN_CLASS + "LZ77");
		return decompressText;
	}
}
