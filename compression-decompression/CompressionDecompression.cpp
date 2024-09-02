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

bool check_path_is_directory(const std::string& fileName) {
	fs::path path(fileName); 
	return fs::is_directory(path);
	
}

double CompressionDecompression::printMemoryUsage() {
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	return pmc.WorkingSetSize / 1024;
}
//compress the data divided to buffers

void CompressionDecompression:: playCompress(const std::string& fileName, const std::string& fileDestination, CompressFunction compressFunc) {

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

// compress the folder and files
void CompressionDecompression::compress(const std::string& fileName, CompressFunction compressFunc) {
	
	// save Memory Usage
	int startMemorySize;
	startMemorySize = printMemoryUsage();
	// save cpu time
	auto start = std::chrono::high_resolution_clock::now();
	Logger::logInfo(Logger::START_FUNCTION + "compress " + Logger::IN_CLASS + "CompressionDecompression");

	//בדיקה אם אתה תקייה או קובץ
	//אם זה תיקיה 
	if (check_path_is_directory(fileName)) {

		fs::path originalPath(fileName);
		fs::path newPath = originalPath.string() + "STZip";
		fs::create_directory(newPath);//יצירה
		// לולאה שעוברת על כל הקבצים בתיקיה
		for (const auto& entry : fs::directory_iterator(originalPath)) {
			// בדיקה אם הערך הנוכחי בלולאה הוא קובץ רגיל ולא תיקיה
			const std::string& fileInFolder=entry.path().string();
			//מקור-entry.path().string();
			//יעד
			const std::string& fileDestination=originalPath.string() + "STZip\\"+entry.path().filename().string();
			if (fs::is_regular_file(entry.status())) {
				CompressionDecompression::playCompress(fileInFolder, fileDestination, compressFunc);
			}
		}

	}
	else {
		CompressionDecompression::playCompress(fileName, fileName, compressFunc);
	}

	// save cpu time
	auto stop = std::chrono::high_resolution_clock::now();
	cpuTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
	// save Memory Usage
	memoryUsage = printMemoryUsage() - startMemorySize;
}


//decompress the data divided to buffers
void CompressionDecompression::decompress(const std::string& path, DecompressFunction decompressFunc) {

	Logger::logInfo(Logger::START_FUNCTION + "decompress " + Logger::IN_CLASS + "CompressionDecompression");

	if (check_path_is_directory(path)) {

		fs::path originalPath(path);
		fs::path newPath = path.substr(0, path.size() - CompressionDecompression::password.size())+"(1)";
		fs::create_directory(newPath);
		// לולאה שעוברת על כל הקבצים בתיקיה
		for (const auto& entry : fs::directory_iterator(originalPath)) {
			// בדיקה אם הערך הנוכחי בלולאה הוא קובץ רגיל ולא תיקיה
			const std::string& fileInFolder = entry.path().string();//remove the chars:(STZip) -> wiki(1)(1).txt
			const std::string& fileDestination = newPath.string();
			if (fs::is_regular_file(entry.status())) {
				CompressionDecompression::playDecompress(fileInFolder, fileDestination,decompressFunc);
			}
		}

	}
	else {
		CompressionDecompression::playDecompress(path, path, decompressFunc);
	}
}

void CompressionDecompression::playDecompress(const std::string& path, const std::string& fileDestination, DecompressFunction compressFunc) {
	IStreamInterface* iStream = new FileStream(path);
	StreamHandler streamHandler(iStream);
	// check if the password is correct
	if (!streamHandler.isCorrectPassword(password))
		Logger::logError(Logger::INVALID_PASSWORD);
	fs::path destinationPath(path);
	iStream->openDestinationStream(fileDestination +"\\"+ destinationPath.filename().string(), false);
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