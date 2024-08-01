#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "LZ77.h"
#include "Huffman.h"
#include "HandleFile.h"

class Deflate
{
	static std::unordered_map<char, std::string> codes;

public:

	static std::string compressDeflate(const std::vector<char>& buffer);
	static std::string compress(const std::string& fileName);
	static std::string decompress(const std::string& text);
};

