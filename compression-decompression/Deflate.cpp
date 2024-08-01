#include "Deflate.h"

std::string Deflate::compressDeflate(const std::vector<char>& buffer) {

	std::vector<char> lz77Compress = LZ77::compress(buffer);
	std::string huffmanCompress = Huffman::compress(lz77Compress);
	return huffmanCompress;
}
std::string Deflate::compress(const std::string& fileName) {

	HandleFile handleFile(fileName);

	//read the buffers
	std::vector<char> buffer;
	std::string compressText;
	while (handleFile.sourceFile.eof()) {
		buffer = handleFile.readBuffer();
		compressText = compressDeflate(buffer);
		handleFile.writeBuffer(codes, compressText);
	}

}
