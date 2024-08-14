#include "CompressionDecompression.h"
#include "Logger.h"
#include "FileStream.h"
#include "StreamHandler.h"
#include "Deflate.h"

const std::string CompressionDecompression::password = "stzip";

//compress the data divided to buffers
void CompressionDecompression::compress(const std::string& fileName, CompressFunction compressFunc) {

	Logger::logInfo(Logger::START_FUNCTION + "compress " + Logger::IN_CLASS + "CompressionDecompression");
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
		compressText = compressFunc(buffer, codes);
		streamHandler.writeBufferCompress(codes, compressText);
	}
	Logger::logInfo(Logger::END_FUNCTION + "compress " + Logger::IN_CLASS + "CompressionDecompression");
	delete iStream;
}


//decompress the data divided to buffers
void CompressionDecompression::decompress(const std::string& fileName, DecompressFunction compressFunc) {

	Logger::logInfo(Logger::START_FUNCTION + "decompress " + Logger::IN_CLASS + "CompressionDecompression");

	IStreamInterface* iStream = new FileStream(fileName);
	StreamHandler streamHandler(iStream);
	// check if the password is correct
	if (!streamHandler.isCorrectPassword(password))
		Logger::logError(Logger::INVALID_PASSWORD);
	iStream->openDestinationStream(fileName, false);
	std::vector<char> buffer;
	std::vector<char>  decompressRes;
	while (streamHandler.getRemainingBytesToRead()) {
		std::unordered_map<char, std::string> codes;
		buffer = streamHandler.readBufferDecompress(codes);
		decompressRes = compressFunc(buffer, codes);
		streamHandler.writeBufferDecompress(decompressRes);
	}
	Logger::logInfo(Logger::END_FUNCTION + "decompress " + Logger::IN_CLASS + "CompressionDecompression");
	delete iStream;
}

