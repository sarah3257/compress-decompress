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

	static std::vector<LZ77Token> getTokens(const std::vector<char>& text) {
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
	// Public interface
	std::vector<char> compress(std::vector<char>& text, std::unordered_map<char, std::string>& codes);
	std::vector<char> decompress(std::vector<char>& text, std::unordered_map<char, std::string>& codesMap);
}
