#include "LZ77.h"
#include "ErrorHandle.h"
#include "HandleFile.h"

int LZ77::maxWindowSize;

// compression

// get a vector of char and return a vector of LZ77Token struct that containing:
// the strings with length, offset and next char 
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

// get tokens - a vector of LZ77Token struct and return a vector of char that containing:
// all the value from the tokens with the character '|' between each character
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

std::vector<char> LZ77::compress(const std::vector<char>& text) {

	std::vector<LZ77Token> tokens = getTokens(text);
	std::vector<char> resultText = changeToString(tokens);
	return resultText;
}

// decompression

// this is fun- helper function  LZ77::decompress to find  from vector <char> a buffer charcter = | 
std::string LZ77::findIndex(const std::vector<char>& vec, int& start) {
	auto it = std::find_if(vec.begin() + start, vec.end(), [](char c) {
		return c == '|';
		});
	if (it == vec.end())
		ErrorHandle::handleError(ErrorHandle::NO_BUFFER_CHARACTER_FOUND);
	int help = start;
	int resIndex = it - vec.begin();
	std::string strRes = std::string(vec.begin() + help, vec.begin() + resIndex);
	start = start + strRes.length() + 1;
	return strRes;

}

std::vector<char> LZ77::decompress(const std::vector<char>& text)
{

	std::vector<char> decompressText;
	for (int i = 0; i < text.size() - 1; i++)
	{
		int offsetText, lengthText;

		offsetText = std::stoi(findIndex(text, i));
		lengthText = std::stoi(findIndex(text, i));
		char nextChar = ' ';
		if (i < text.size())
			nextChar = text[i++];
		int start = decompressText.size() - offsetText;

		for (int copyText = start; copyText < lengthText + start; copyText++) {
			decompressText.push_back(decompressText[copyText]);
		}
		decompressText.push_back(nextChar);

	}
	return decompressText;
}
