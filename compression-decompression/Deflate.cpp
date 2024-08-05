#include "Deflate.h"

std::string Deflate::compressDeflate(const std::vector<char>& buffer, std::unordered_map<char, std::string>& codes) {

	std::vector<char> lz77Compress = LZ77::compress(buffer);
	std::string huffmanCompress = Huffman::compress(codes,lz77Compress);
	return huffmanCompress;
}
void Deflate::compress(const std::string& fileName) {

	HandleFile handleFile(fileName, true);

	//read the buffers
	std::vector<char> buffer;
	std::string compressText;
	while (handleFile.getFileSizeMinusCurrentSize()) {
	//for(int i=0; i<1;i++){
	//while (!handleFile.getSourceFileEOF()) {
		buffer = handleFile.readBufferCompress();
		std::unordered_map<char, std::string> codes;
		compressText = compressDeflate(buffer,codes);
		//עד כאן טוב
		handleFile.writeBufferCompress(codes, compressText);
	}

}
void Deflate::decompress(const std::string& text) {
	std::vector<char> buffer;
	std::vector<char>  decompressRes;
	HandleFile handleFile(text, false);
	while (handleFile.getFileSizeMinusCurrentSize()) {
		std::unordered_map<char, std::string> codes;
		buffer = handleFile.readBufferDecompress(codes);
		int sss = handleFile.getFileSizeMinusCurrentSize();
		// we need get a code map
		decompressRes = decompressDeflate(buffer, codes);
		handleFile.writeBufferDecompress(decompressRes);
	}
}

std::vector<char> Deflate::decompressDeflate(const std::vector<char>& buffer, std::unordered_map<char, std::string>& codes)
{
	std::vector<char> decompressHuffman =Huffman::decompress(codes, buffer);
	std::vector<char>decompressLZ77 = LZ77::decompress(decompressHuffman);
	return decompressLZ77;
}
