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

const std::string CompressionDecompression::password = "STZip";
double CompressionDecompression::cpuTime = 0.0;
double CompressionDecompression::memoryUsage = 0.0;



double CompressionDecompression::printMemoryUsage() {
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	return static_cast<double>(pmc.WorkingSetSize / 1024);
}
//compress the data divided to buffers

void CompressionDecompression::playCompress(const std::string& fileName, const std::string& fileDestination, CompressFunction compressFunc) {

	IStreamInterface* iStream = new FileStream(fileName);
	StreamHandler streamHandler(iStream);
	iStream->openDestinationStream(fileDestination, true);
	streamHandler.insertPassword(password);
	//-----------------------start to devide to files
	streamHandler.insertFileExtension(fileDestination);
	//read the buffers
	std::vector<char> buffer;
	std::vector<char> compressText;
	while (streamHandler.getRemainingBytesToRead()) {
		buffer = streamHandler.readBufferCompress();
		std::unordered_map<char, std::string> codes;
		compressText = compressFunc(buffer, codes);
		streamHandler.writeBufferCompress(codes, compressText);
	}
	Logger::logInfo(Logger::END_FUNCTION + "compress " + Logger::IN_CLASS + "CompressionDecompression");
	delete iStream;
}

void CompressionDecompression::compressRec(const std::string& filesource, const std::string& fileDestination, CompressFunction compressFunc) {
	fs::path originalPath(filesource);
	fs::path DesPath(fileDestination);
	if (!fs::is_directory(originalPath)) {//לבדוק אולי מומלץ לבדוק אם זה קובץ השאלה מה זה קובץ רגיל

		CompressionDecompression::playCompress(filesource, fileDestination, compressFunc);
		return;
	}
	fs::path newPath = DesPath.string() + "STZip";
	fs::create_directory(newPath);
	for (const auto& entry : fs::directory_iterator(originalPath)) {

		compressRec(entry.path().string(), DesPath.string() + "STZip\\" + entry.path().filename().string(), compressFunc);
	}
}

// compress the folder and files
void CompressionDecompression::compress(const std::string& fileName, CompressFunction compressFunc) {
	// save Memory Usage
	int startMemorySize;
	startMemorySize = static_cast<int>(printMemoryUsage());
	// save cpu time
	auto start = std::chrono::high_resolution_clock::now();
	Logger::logInfo(Logger::START_FUNCTION + "compress " + Logger::IN_CLASS + "CompressionDecompression");
	compressRec(fileName, fileName, compressFunc);

	// save cpu time
	auto stop = std::chrono::high_resolution_clock::now();
	cpuTime = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count());
	// save Memory Usage
	memoryUsage = printMemoryUsage() - startMemorySize;

}

//decompress the data divided to buffers
void CompressionDecompression::decompress(const std::string& path, DecompressFunction decompressFunc) {

	Logger::logInfo(Logger::START_FUNCTION + "decompress " + Logger::IN_CLASS + "CompressionDecompression");
	deCompressRec(path, path, decompressFunc);
}

void CompressionDecompression::playDecompress(const std::string& path, const std::string& fileDestination, DecompressFunction compressFunc) {
	IStreamInterface* iStream = new FileStream(path);
	StreamHandler streamHandler(iStream);
	// check if the password is correct
	if (!streamHandler.isCorrectPassword(password))
		Logger::logError(Logger::INVALID_PASSWORD);
	fs::path destinationPath(path);
	iStream->openDestinationStream(fileDestination, false);
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

void CompressionDecompression::deCompressRec(const std::string& filesource, const std::string& fileDestination, DecompressFunction deCompressFunc) {

	fs::path originalPath(filesource);
	fs::path DesPath(fileDestination);
	if (!fs::is_directory(originalPath)) {
		CompressionDecompression::playDecompress(filesource, fileDestination, deCompressFunc);
		return;
	}
	// create new path in 3 steps
	// 1. all the path without the file name
	// 2. add the file name without the exteion :STZip
	// 3. add the extion (1)
	fs::path newPath = fileDestination.substr(0, fileDestination.size() - DesPath.filename().string().size()) + originalPath.filename().string().substr(0, originalPath.filename().string().size() - CompressionDecompression::password.size()) + "(1)";
	fs::create_directory(newPath);

	// a loop for all the files in the folder
	for (const auto& entry : fs::directory_iterator(originalPath)) {
		deCompressRec(entry.path().string(), newPath.string() + "\\" + entry.path().filename().string(), deCompressFunc);
	}
}
