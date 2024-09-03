#pragma once
#include <string>
#include <vector>
#include <unordered_map>
class CompressionDecompression
{
public:
	static const std::string password;
	static double cpuTime;
	static double memoryUsage;
	using CompressFunction = std::vector<char>(*)( std::vector<char>&, std::unordered_map<char, std::string>&);
	using DecompressFunction = std::vector<char>(*)( std::vector<char>&, std::unordered_map<char, std::string>&);
	static double printMemoryUsage();
	static void compress(const std::string& fileName, CompressFunction compressFunc);
	static void decompress(const std::string& path, DecompressFunction compressFunc);
	static void playCompress(const std::string& fileName, const std::string& fileDestination, CompressFunction compressFunc) ;
	static void playDecompress(const std::string& fileName, const std::string& fileDestination, DecompressFunction compressFunc);
	static void compressRec(const std::string& filesource, const std::string& fileDestination, CompressFunction deCompressFunc);
	static void deCompressRec(const std::string& filesource, const std::string& fileDestination, DecompressFunction deCompressFunc);

};

