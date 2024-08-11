#include "Deflate.h"
#include "ErrorHandle.h"
#include "FileStream.h"
#include "StreamHandler.h"
#include <fstream>
#include <iostream>

const std::string Deflate::password = "stzip";

//compress one buffer
std::string Deflate::compressDeflate(const std::vector<char>& buffer, std::unordered_map<char, std::string>& codes) {

	std::vector<char> lz77Compress = LZ77::compress(buffer);
	std::string huffmanCompress = Huffman::compress(codes, lz77Compress);
	return huffmanCompress;
}

//compress the data divided to buffers
void Deflate::compress(const std::string& fileName) {

	IStreamInterface* iStream = new FileStream(fileName);
	StreamHandler streamHandler(iStream);
	iStream->openDestinationStream(fileName, true);
	streamHandler.insertPassword(password);
	streamHandler.insertFileExtension(fileName);
	//read the buffers
	std::vector<char> buffer;
	std::string compressText;
	while (streamHandler.getRemainingBytesToRead()) {
		buffer = streamHandler.readBufferCompress();
		std::unordered_map<char, std::string> codes;
		compressText = compressDeflate(buffer, codes);
		streamHandler.writeBufferCompress(codes, compressText);
	}
}

//decompress the data divided to buffers
void Deflate::decompress(const std::string& fileName) {

	IStreamInterface* iStream = new FileStream(fileName);
	StreamHandler streamHandler(iStream);
	// check if the password is correct
	if (!streamHandler.isCorrectPassword(password))
		ErrorHandle::handleError(ErrorHandle::INVALID_PASSWORD);
	
	iStream->openDestinationStream(fileName,false);
	std::vector<char> buffer;
	std::vector<char>  decompressRes;
	while (streamHandler.getRemainingBytesToRead()) {
		std::unordered_map<char, std::string> codes;
		buffer = streamHandler.readBufferDecompress(codes);
		decompressRes = decompressDeflate(buffer, codes);
		streamHandler.writeBufferDecompress(decompressRes);
	}

}

//decompress one buffer
std::vector<char> Deflate::decompressDeflate(const std::vector<char>& buffer, std::unordered_map<char, std::string>& codes)
{
	std::vector<char> decompressHuffman = Huffman::decompress(codes, buffer);
	std::vector<char>decompressLZ77 = LZ77::decompress(decompressHuffman);
	return decompressLZ77;
}
