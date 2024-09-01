#include "FileStream.h"
#include "Logger.h"
#include "StreamHandler.h"
#include <bitset>
#include <string_view>
#define M 100

double FileStream::EfficiencyPercentages = 0.0;

FileStream::FileStream(const std::string& sourceFilePath) {

	sourceFile.open(sourceFilePath, std::ios::binary);
	if (!sourceFile)
		Logger::logError(Logger::CANNOT_OPEN_FILE);
}

FileStream::~FileStream() {
	int destinationSize = 0, sourceSize = 0;
	if (sourceFile.is_open()) {
		sourceSize = sourceFile.tellg();
		sourceFile.close();
	}
	if (destinationFile.is_open()) {
		destinationSize= destinationFile.tellp();
		destinationFile.close();
	}
	if (sourceSize != 0)
		EfficiencyPercentages = M - (destinationSize * M / sourceSize);
}

void FileStream::openDestinationStream(const std::string& sourceNamae, bool isCompress) {

	std::string destinationFilePath = readFileName(sourceNamae);
	std::string zipExtension = "(zip)";

	if (isCompress)
		destinationFilePath += zipExtension + ".bin";
	else {
		destinationFilePath = destinationFilePath.substr(0, destinationFilePath.size() - zipExtension.size());
		destinationFilePath += "(1)" + readFileExtension();
	}

	destinationFile.open(destinationFilePath, std::ios::binary);
	if (!destinationFile)
		Logger::logError(Logger::CANNOT_OPEN_FILE);
}

void FileStream::readData(std::vector<char>& buffer) {
	if (!sourceFile)
		Logger::logError(Logger::CANNOT_OPEN_FILE);
	sourceFile.read(buffer.data(), buffer.size());
}

void FileStream::readData(int& dataSize) {
	if (!sourceFile)
		Logger::logError(Logger::CANNOT_OPEN_FILE);
	sourceFile.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
	if (sourceFile.gcount() != sizeof(dataSize))
		Logger::logError(Logger::FAILED_READ_DATA_SIZE_FROM_FILE);
}

void FileStream::writeData(const std::vector<char>& buffer) {

	if (!destinationFile)
		Logger::logError(Logger::CANNOT_OPEN_FILE);
	// Write the vector content as a single string to the file
	destinationFile.write(buffer.data(), buffer.size());
	if (destinationFile.fail())
		Logger::logError(Logger::FAILED_WRITE_TO_FILE);
}

void FileStream::writeData(int& size) {
	if (!destinationFile.is_open())
		Logger::logError(Logger::CANNOT_OPEN_FILE);
	destinationFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
	if (destinationFile.fail())
		Logger::logError(Logger::FAILED_WRITE_TO_FILE);
}

void FileStream::writeMap(const std::unordered_map<char, std::string>& codes) {
	//push map.size and map
	int mapSize = codes.size();
	writeData(mapSize);
	for (const auto& pair : codes) {
		destinationFile.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));
		int strSize = pair.second.size();
		std::string text = pair.second;
		while (text.size() % 8)
			text.push_back('0');
		std::vector<char> buffer;
		for (int i = 0; i < text.size(); i += 8) {
			std::bitset<8> byte(std::string(text.data() + i, 8));
			buffer.push_back(static_cast<char>(byte.to_ulong()));
		}
		writeData(strSize);
		writeData(buffer);
	}
}

void FileStream::readMap(std::unordered_map<char, std::string>& codes) {
	int mapSize;
	int valueSize;
	char key;

	// read the size of the map
	sourceFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
	if (sourceFile.gcount() != sizeof(mapSize))
		Logger::logError(Logger::FAILED_READ_4_BYTES_FROM_FILE);

	// read the map from the file to an unordered_map
	for (int i = 0; i < mapSize; ++i) {
		// read the key
		sourceFile.read(reinterpret_cast<char*>(&key), sizeof(key));
		if (sourceFile.gcount() != sizeof(key))
			Logger::logError(Logger::FAILED_READ_KEY_FROM_FILE);
		// read the value size
		sourceFile.read(reinterpret_cast<char*>(&valueSize), sizeof(valueSize));
		if (sourceFile.gcount() != sizeof(valueSize))
			Logger::logError(Logger::FAILED_READ_VALUE_SIZE_FROM_FILE);
		// read the value
		int bufferSize = (valueSize + 7) / 8;
		std::vector<char> dataBuffer(bufferSize);
		readData(dataBuffer);

		// return the value
		std::vector<char> binaryBuffer = StreamHandler::convertToBinaryVector(dataBuffer);
		for (int i = 0; i < bufferSize * 8 - valueSize; i++)
			binaryBuffer.pop_back();
		std::string value(binaryBuffer.begin(), binaryBuffer.end());
		// insert the value to the unordered_map
		codes[key] = value;
	}
}

int FileStream::getRemainingBytesToRead() {
	std::streampos current_pos = sourceFile.tellg();
	sourceFile.seekg(0, std::ios::end);
	std::streampos file_size = sourceFile.tellg();
	sourceFile.seekg(current_pos);
	int remaining_size = file_size - current_pos;
	return remaining_size;
}

long long FileStream::getSourceSize() {
	std::streampos currentPos = sourceFile.tellg();
	sourceFile.seekg(0, std::ios::end);
	std::streampos fileSize = sourceFile.tellg();
	sourceFile.seekg(currentPos);
	return fileSize;
}

//read file name
std::string FileStream::readFileName(const std::string& fileName) {
	int pos = fileName.find_last_of('.');
	if (pos == std::string::npos)
		Logger::logError(Logger::NO_FILE_NAME_FOUND);
	return fileName.substr(0, pos);
}

//read file extension
std::string FileStream::readFileExtension() {
	if (!sourceFile.is_open())
		Logger::logError(Logger::CANNOT_OPEN_FILE);
	// Read the size of the extension from the file
	int extensionSize = 0;
	sourceFile.read(reinterpret_cast<char*>(&extensionSize), sizeof(int));
	if (sourceFile.fail())
		Logger::logError(Logger::FAILED_READ_EXTENSION_SIZE_FROM_FILE);

	std::vector<char> fileExtension(extensionSize);
	readData(fileExtension);
	if (fileExtension.empty())
		Logger::logError(Logger::FAILED_READ_EXTENSION_FROM_FILE);
	std::string fileExtensionString(fileExtension.begin(), fileExtension.end());
	return fileExtensionString;
}

