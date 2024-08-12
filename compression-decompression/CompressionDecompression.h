#pragma once
#include <string>
class CompressionDecompression
{
	static const std::string password;
public:
	static void compress(const std::string& fileName);
	static void decompress(const std::string& fileName);
};

