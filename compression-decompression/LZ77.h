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

    int maxWindowSize;

    // Public interface
    std::vector<char> compress(const std::vector<char>& text, const std::unordered_map<char, std::string>& codes);
    std::vector<char> decompress(const std::vector<char>& text, const std::unordered_map<char, std::string>& codesMap);
}
