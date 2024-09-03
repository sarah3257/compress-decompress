#pragma once
#include <vector>
#include <string>
#include <unordered_map>
struct LZ77Token {
	int offset;
	int length;
	char nextChar;
	LZ77Token(int offset, int length, char nextChar)
		: offset(offset), length(length), nextChar(nextChar) {}
	bool operator==(const LZ77Token& other)const {
		return other.length == length && other.nextChar == nextChar && other.offset == offset;
	}
};
class LZ77 {
	static std::vector<char> changeToString(const std::vector<LZ77Token>& tokens);
	static std::string findIndex(const std::vector<char>& vec, int& start);
public:
	static std::vector<LZ77Token> getTokens(const std::vector<char>& text);
	static int maxWindowSize;
	static std::vector<char> compress(std::vector<char>& text, std::unordered_map<char, std::string>& codes);
	static std::vector<char> decompress(std::vector<char>& text, std::unordered_map<char, std::string>& codesMap);
};
