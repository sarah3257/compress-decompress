#include "Deflate.h"

//compress one buffer
std::string Deflate::compress( std::vector<char>& buffer, std::unordered_map<char, std::string>& codes) {
	std::string lz77CompressString = LZ77::compress(buffer, codes);
	std::vector<char> lz77Compress = std::vector<char>(lz77CompressString.begin(), lz77CompressString.end());;
	std::string huffmanCompress = Huffman::compress(lz77Compress, codes);
	return huffmanCompress;
}

//decompress one buffer
std::vector<char> Deflate::decompress( std::vector<char>& buffer, std::unordered_map<char, std::string>& codes)
{
	std::vector<char> decompressHuffman = Huffman::decompress(buffer, codes);
	std::vector<char>decompressLZ77 = LZ77::decompress(decompressHuffman, codes);
	return decompressLZ77;
}
