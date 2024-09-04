#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "BitString.h"
struct LZ77Token {
	int offset;
	int length;
	char nextChar;
	LZ77Token(int offset, int length, char nextChar)
		: offset(offset), length(length), nextChar(nextChar) {}
	bool operator==(const LZ77Token& other) const {
		return offset == other.offset && length == other.length && nextChar == other.nextChar;
	}
};
class LZ77 {
	static std::vector<char> changeToString(const std::vector<LZ77Token>& tokens);
	static std::string findIndex(const std::vector<char>& vec, int& start);
public:
	static std::vector<LZ77Token> getTokens(const std::vector<char>& text);
	static int maxWindowSize;
	static std::vector<char> compress(std::vector<char>& text, std::unordered_map<char, BitString>& codes);
	static std::vector<char> decompress(std::vector<char>& text, std::unordered_map<char, BitString>& codesMap);
};
