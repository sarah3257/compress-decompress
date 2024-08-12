#pragma once
#include <string>
#include <vector>
#include "LZ77.h"
#include "Huffman.h"

class Deflate
{
public:
	static std::string compress(const std::vector<char>& buffer, std::unordered_map<char, std::string>& codes);
	static std::vector<char>  decompress(const std::vector<char>& buffer, std::unordered_map<char, std::string>& codes);
};

