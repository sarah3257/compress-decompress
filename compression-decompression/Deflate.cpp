#include "Deflate.h"

//compress one buffer
std::string Deflate::compress(const std::vector<char>& buffer, std::unordered_map<char, std::string>& codes) {
	std::vector<char> lz77Compress = LZ77::compress(buffer);
	std::string huffmanCompress = Huffman::compress(codes, lz77Compress);
	return huffmanCompress;
}

//decompress one buffer
std::vector<char> Deflate::decompress(const std::vector<char>& buffer, std::unordered_map<char, std::string>& codes)
{
	std::vector<char> decompressHuffman = Huffman::decompress(codes, buffer);
	std::vector<char>decompressLZ77 = LZ77::decompress(decompressHuffman);
	return decompressLZ77;
}
