#pragma once
#include <string>
#include <vector>
#include <unordered_map>
class CompressionDecompression
{
	static const std::string password;
public:
	using CompressFunction = std::string(*)(const std::vector<char>&, std::unordered_map<char, std::string>&);
	using DecompressFunction = std::vector<char>(*)(const std::vector<char>&, std::unordered_map<char, std::string>&);
	static void compress(const std::string& fileName, CompressFunction compressFunc);
	static void decompress(const std::string& fileName, DecompressFunction compressFunc);
};

