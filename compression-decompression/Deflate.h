#pragma once
#include <string>
#include <vector>
#include "LZ77.h"
#include "Huffman.h"

namespace Deflate
{
	 std::string compress( std::vector<char>& buffer, std::unordered_map<char, std::string>& codes);
	 std::vector<char>  decompress( std::vector<char>& buffer, std::unordered_map<char, std::string>& codes);
};

