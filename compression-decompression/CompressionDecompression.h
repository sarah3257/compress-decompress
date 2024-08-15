#pragma once
#include <string>
#include <vector>
#include <unordered_map>
class CompressionDecompression
{
	static const std::string password;
public:
	static double cpuTime;
	static double memoryUsage;
	using CompressFunction = std::string(*)( std::vector<char>&, std::unordered_map<char, std::string>&);
	using DecompressFunction = std::vector<char>(*)( std::vector<char>&, std::unordered_map<char, std::string>&);
	static double printMemoryUsage();
	static void compress(const std::string& fileName, CompressFunction compressFunc);
	static void decompress(const std::string& fileName, DecompressFunction compressFunc);
};

