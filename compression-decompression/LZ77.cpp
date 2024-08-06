#include "LZ77.h"

//compression
std::vector<LZ77Token> LZ77::getTokens(const std::vector<char>& text) {

	std::vector<LZ77Token> tokens;
	int i = 0;
	int maxMatchDistance, maxMatchLength, startWindow;

	// a loop for every char to find a srting
	while (i < text.size()) {
		maxMatchDistance = 0;
		maxMatchLength = 0;
		startWindow = i < MAX_WINDOW_SIZE ? 0 : i - MAX_WINDOW_SIZE;

		// for every char check backwards the max string
		for (int j = startWindow; j < i && maxMatchLength < i - j; j++) {
			int matchLength = 0;
			while (text[i + matchLength] == text[j + matchLength]) {
				matchLength++;
				if (i + matchLength >= text.size() || j + matchLength >= i) {
					break;
				}
			}

			if (matchLength > maxMatchLength) {
				maxMatchLength = matchLength;
				maxMatchDistance = i - j;
			}

		}
		// insert the distance length & next char to the token
		if (i + maxMatchLength == text.size()) {
			tokens.emplace_back(maxMatchDistance, maxMatchLength - 1, text[i + maxMatchLength - 1]);
		}
		else
			tokens.emplace_back(maxMatchDistance, maxMatchLength, text[i + maxMatchLength]);

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

	//text.pop_back();
	return text;
}
std::vector<char> LZ77::compress(const std::vector<char>& text) {

	//main
	/*LZ77 lz;
	std::vector<char> text;
	text.push_back('A');
	text.push_back('B');
	text.push_back('A');
	text.push_back('B');
	text.push_back('l');
	std::vector<char> res = lz.compress(text);
	for (int i = 0; i < res.size(); i++) {
		std::cout << res[i];
	}*/

	std::vector<LZ77Token> tokens = getTokens(text);
	std::vector<char> resultText = changeToString(tokens);
	return resultText;
}

//decompression

//this is fun- helper function  LZ77::decompress to find  from vector <char> a buffer charcter = | 
std::string LZ77::findIndex(std::vector<char> vec, int& start) {
	auto it = std::find_if(vec.begin() + start, vec.end(), [](char c) {
		return c == '|';
		});
	if (it == vec.end())
		std::cout << "We need do it---ErrorHandle::NO_BUFFER_CHARACTER_FOUND";
	int help = start;
	int resIndex = it - vec.begin();
	std::string strRes = std::string(vec.begin() + help, vec.begin() + resIndex);
	start = start + strRes.length() + 1;
	return strRes;

}

//int main() {
//	std::vector<char> text = { '0','|','0','|','A','|','0','|','0','|','B','|','2','|','2','|','l' };
//	std::vector<char> res = LZ77::decompress(text);
//	for (int i = 0; i < res.size(); i++) {
//		std::cout << res[i];
//	}
//}

std::vector<char> LZ77::decompress(const std::vector<char> text)
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







