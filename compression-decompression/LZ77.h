#pragma once
#include <vector>
#include <string>
#include <unordered_map>

namespace LZ77 {
	struct LZ77Token {
		int offset;
		int length;
		char nextChar;
		LZ77Token(int offset, int length, char nextChar)
			: offset(offset), length(length), nextChar(nextChar) {}
	};

	extern int maxWindowSize;

	// Public interface
	std::vector<char> compress(std::vector<char>& text, std::unordered_map<char, std::string>& codes);
	std::vector<char> decompress(std::vector<char>& text, std::unordered_map<char, std::string>& codesMap);
}
