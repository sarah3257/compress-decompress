#pragma once
#include <vector>
#include <string>
#include <iostream>

const int MAX_WINDOW_SIZE = 8192;  // Maximum window size 8KB
//const int MAX_WINDOW_SIZE = 32768; // 32KB

struct LZ77Token {
	int offset;
	int length;
	char nextChar;
	LZ77Token(int offset, int length, char nextChar)
		: offset(offset), length(length), nextChar(nextChar) {}
};

class LZ77
{	
	static std::vector<LZ77Token> getTokens(const std::vector<char>& text);
	static std::vector<char> changeToString(const std::vector<LZ77Token>& tokens);
	std::string findIndex(std::vector<char> vec, int& start);

public:
	static std::vector<char> compress(const std::vector<char>& text);
	static std::string decompress(const std::string& text);
};

