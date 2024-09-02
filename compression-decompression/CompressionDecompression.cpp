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

namespace fs = std::filesystem; // שימוש בקיצור למרחב השמות של מערכת הקבצים

bool check_path_is_directory(const std::string& fileName) {
	fs::path path(fileName); // המרת מחרוזת ל- fs::path לצורך עבודה עם מערכת הקבצים
	return fs::is_directory(path);
}

const std::string CompressionDecompression::password = "stzip";
double CompressionDecompression::cpuTime = 0.0;
double CompressionDecompression::memoryUsage = 0.0;

double CompressionDecompression::printMemoryUsage() {
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	return pmc.WorkingSetSize / 1024;
}
void CompressionDecompression:: play(const std::string& fileName, const std::string& fileDestination, CompressFunction compressFunc) {

	// save Memory Usage
	int startMemorySize;
	startMemorySize = printMemoryUsage();
	// save cpu time
	auto start = std::chrono::high_resolution_clock::now();
	Logger::logInfo(Logger::START_FUNCTION + "compress " + Logger::IN_CLASS + "CompressionDecompression");

	IStreamInterface* iStream = new FileStream(fileName);
	StreamHandler streamHandler(iStream);
	iStream->openDestinationStream(fileDestination, true);
	streamHandler.insertPassword(password);
	//-----------------------start to devide to files
	streamHandler.insertFileExtension(fileDestination);
	//read the buffers
	std::vector<char> buffer;
	std::vector<char> compressText;
	while (streamHandler.getRemainingBytesToRead()) {//if remainig another files
		buffer = streamHandler.readBufferCompress();
		std::unordered_map<char, std::string> codes;
		compressText = compressFunc(buffer, codes);
		streamHandler.writeBufferCompress(codes, compressText);
	}
	Logger::logInfo(Logger::END_FUNCTION + "compress " + Logger::IN_CLASS + "CompressionDecompression");
	// save cpu time
	auto stop = std::chrono::high_resolution_clock::now();
	cpuTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
	// save Memory Usage
	memoryUsage = printMemoryUsage() - startMemorySize;
}

//compress the data divided to buffers
void CompressionDecompression::compress(const std::string& fileName, CompressFunction compressFunc) {
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
				CompressionDecompression::play(fileInFolder, fileDestination, compressFunc);
			}
		}

	}
	else {
		CompressionDecompression::play(fileName, fileName, compressFunc);
	}

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

