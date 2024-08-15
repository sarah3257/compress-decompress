#pragma once
#include <string>
#include <vector>
#include "LZ77.h"
#include "Huffman.h"

class Deflate
{
public:
	static std::vector<char> compress(std::vector<char>& buffer, std::unordered_map<char, std::string>& codes);
	static std::vector<char>  decompress(std::vector<char>& buffer, std::unordered_map<char, std::string>& codes);
};