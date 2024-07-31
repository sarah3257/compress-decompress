#pragma once
#include <vector>
#include <string>

struct LZ77Token {
	int offset;
	int length;
	char nextChar;
	LZ77Token(int offset, int length, char nextChar)
		: offset(offset), length(length), nextChar(nextChar) {}
};

class LZ77
{	
	std::vector<LZ77Token> getTokens(const std::string& text);
	std::string changeToString(const std::vector<LZ77Token>& tokens);
public:
	static std::string compress(const std::string& text);
	static std::string decompress(const std::string& text);
};

