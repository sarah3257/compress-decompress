#include "Deflate.h"
#include "ErrorHandle.h"
#include <fstream>
#include <iostream>
#include "ErrorHandle.h"
#include "Logger.h"

const std::string Deflate::password = "stzip";

//compress one buffer
std::string Deflate::compressDeflate(const std::vector<char>& buffer, std::unordered_map<char, std::string>& codes) {
	std::vector<char> lz77Compress = LZ77::compress(buffer);
	std::string huffmanCompress = Huffman::compress(codes, lz77Compress);
	return huffmanCompress;
}

//compress the data divided to buffers
void Deflate::compress(const std::string& fileName) {
	Logger::logInfo(Logger::START_FUNCTION + "compress " + Logger::IN_CLASS + "Deflate");
	HandleFile handleFile(fileName);
	handleFile.insertPassword(password);
	handleFile.insertFileExtension(fileName);
	//read the buffers
	std::vector<char> buffer;
	std::string compressText;
	while (handleFile.getRemainingBytesToRead()) {
		buffer = handleFile.readBufferCompress();
		std::unordered_map<char, std::string> codes;
		compressText = compressDeflate(buffer, codes);
		handleFile.writeBufferCompress(codes, compressText);
	}
	Logger::logInfo(Logger::END_FUNCTION + "compress " + Logger::IN_CLASS + "Deflate");
}

//decompress the data divided to buffers
void Deflate::decompress(const std::string& text) {
	Logger::logInfo(Logger::START_FUNCTION + "decompress " + Logger::IN_CLASS + "Deflate");
	// check if the password is correct
	if (!HandleFile::isCorrectPassword(text, password))
		ErrorHandle::handleError(ErrorHandle::INVALID_PASSWORD);
	else {
		std::vector<char> buffer;
		std::vector<char>  decompressRes;
		HandleFile handleFile(text, false, password.size());
		while (handleFile.getRemainingBytesToRead()) {
			std::unordered_map<char, std::string> codes;
			buffer = handleFile.readBufferDecompress(codes);
			decompressRes = decompressDeflate(buffer, codes);
			handleFile.writeBufferDecompress(decompressRes);
		}
	}
	Logger::logInfo(Logger::END_FUNCTION + "decompress " + Logger::IN_CLASS + "Deflate");
}

//decompress one buffer
std::vector<char> Deflate::decompressDeflate(const std::vector<char>& buffer, std::unordered_map<char, std::string>& codes)
{
	std::vector<char> decompressHuffman = Huffman::decompress(codes, buffer);
	std::vector<char>decompressLZ77 = LZ77::decompress(decompressHuffman);
	return decompressLZ77;
}
