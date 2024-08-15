#include "Deflate.h"

//compress one buffer
std::vector<char> Deflate::compress( std::vector<char>& buffer, std::unordered_map<char, std::string>& codes) {
	std::vector<char> lz77Compress = LZ77::compress(buffer, codes);
	lz77Compress.pop_back();
	std::vector<char> huffmanCompress = Huffman::compress(lz77Compress, codes);
	return huffmanCompress;
}

//decompress one buffer
std::vector<char> Deflate::decompress( std::vector<char>& buffer, std::unordered_map<char, std::string>& codes)
{
	std::vector<char> decompressHuffman = Huffman::decompress(buffer, codes);
	std::vector<char>decompressLZ77 = LZ77::decompress(decompressHuffman, codes);
	return decompressLZ77;
}
