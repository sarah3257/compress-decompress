#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "LZ77.h"
#include "Huffman.h"

class Deflate
{
	std::ifstream& file;

public:
	~Deflate()
	{
		// close the file
		file.close();
	}
	static std::vector<char> readBuffer(const std::ifstream& file);
	static std::string compressDeflate(const std::vector<char>& buffer);
	static std::string compress(const std::string& fileName);
	static std::string decompress(const std::string& text);
};

