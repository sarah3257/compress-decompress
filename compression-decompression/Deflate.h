#pragma once
#include <string>
#include <vector>
#include "LZ77.h"
#include "Huffman.h"
#include "HandleFile.h"

class Deflate
{
	static const std::string password;

public:
	static std::string compressDeflate(const std::vector<char>& buffer, std::unordered_map<char, std::string>& codes);
	static void compress(const std::string& fileName);
	static void decompress(const std::string& text);
	static std::vector<char>  decompressDeflate(const std::vector<char>& buffer, std::unordered_map<char, std::string>& codes);
};

