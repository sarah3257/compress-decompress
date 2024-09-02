#include "CompressionDecompression.h"
#include "Logger.h"
#include "FileStream.h"
#include "StreamHandler.h"
#include "Deflate.h"
#include <iostream>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <filesystem>

namespace fs = std::filesystem;

const std::string CompressionDecompression::password = "stzip";
double CompressionDecompression::cpuTime = 0.0;
double CompressionDecompression::memoryUsage = 0.0;

double CompressionDecompression::printMemoryUsage() {
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	return pmc.WorkingSetSize / 1024;
}

//compress the data divided to buffers
void CompressionDecompression::compress(const std::string& fileName, CompressFunction compressFunc) {

	fs::path path(fileName);
	// save Memory Usage
	int startMemorySize;
	startMemorySize = printMemoryUsage();
	// save cpu time
	auto start = std::chrono::high_resolution_clock::now();
	Logger::logInfo(Logger::START_FUNCTION + "compress " + Logger::IN_CLASS + "CompressionDecompression");
	/////////
	if (fs::is_directory(path)) {
		std::string newPath = fileName + "(zip)";
		fs::create_directory(newPath);
		for (const auto& entry : fs::directory_iterator(path)) {
			if (fs::is_regular_file(entry)) {
				IStreamInterface* iStream = new FileStream(entry.path().string());
				StreamHandler streamHandler(iStream);
				iStream->openDestinationStream(newPath + "\\" + entry.path().filename().string(), true);
				streamHandler.insertPassword(password);
				streamHandler.insertFileExtension(newPath + "\\" + entry.path().filename().string());
				//read the buffers
				std::vector<char> buffer;
				std::vector<char> compressText;
				while (streamHandler.getRemainingBytesToRead()) {
					buffer = streamHandler.readBufferCompress();
					std::unordered_map<char, std::string> codes;
					compressText = compressFunc(buffer, codes);
					streamHandler.writeBufferCompress(codes, compressText);
				}
				delete iStream;
			}
		}
	}
	else {
		IStreamInterface* iStream = new FileStream(fileName);
		StreamHandler streamHandler(iStream);
		iStream->openDestinationStream(fileName, true);
		streamHandler.insertPassword(password);
		streamHandler.insertFileExtension(fileName);
		//read the buffers
		std::vector<char> buffer;
		std::vector<char> compressText;
		while (streamHandler.getRemainingBytesToRead()) {
			buffer = streamHandler.readBufferCompress();
			std::unordered_map<char, std::string> codes;
			compressText = compressFunc(buffer, codes);
			streamHandler.writeBufferCompress(codes, compressText);
		}
		delete iStream;
	}
	Logger::logInfo(Logger::END_FUNCTION + "compress " + Logger::IN_CLASS + "CompressionDecompression");
	// save cpu time
	auto stop = std::chrono::high_resolution_clock::now();
	cpuTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
	// save Memory Usage
	memoryUsage = printMemoryUsage() - startMemorySize;
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

