#include "StreamHandler.h"
#include "LZ77.h"
#include "IStreamInterface.h"
#include "Logger.h"
#include <algorithm>
#include <cmath>
#include <bitset>

//open the source and destination file
StreamHandler::StreamHandler(IStreamInterface * streamInterface):streamInterface(streamInterface){
	setMaxWindowSize();
}

//read buffer from file to compress
std::vector<char> StreamHandler::readBufferCompress() {

	int remainingSize = getRemainingBytesToRead();
	int size_buffer = std::min(remainingSize, BUFFER_SIZE);
	std::vector<char> bufferText(size_buffer);
	streamInterface->readData(bufferText, size_buffer);
	return bufferText;
}

//write to file the compressed buffer
void StreamHandler::writeBufferCompress(const std::unordered_map<char, std::string>& codes, std::string& text) {
	
	// write the map
	streamInterface->writeMap(codes);
	//push data.size and data
	int dataSize = text.size();
	while (text.size() % 8)
		text.push_back('0');
	std::vector<char> buffer;
	for (int i = 0; i < text.size(); i += 8) {
		std::string byteString = text.substr(i, 8);
		std::bitset<8> byte(byteString);
		buffer.push_back(static_cast<char>(byte.to_ulong()));
	}
	streamInterface->writeData(dataSize);
	streamInterface->writeData(buffer);
}

//convert char vector to binary vector
std::vector<char> StreamHandler::convertToBinaryVector(const std::vector<char>& dataBuffer) {
	std::vector<char> binaryBuffer;
	for (char ch : dataBuffer) {
		std::bitset<8> binary(ch);
		for (std::size_t i = 0; i < 8; ++i)
			binaryBuffer.push_back(binary.test(7 - i) ? '1' : '0');
	}
	return binaryBuffer;
}

//read buffer from file to decompress and fill the codesMap and data
std::vector<char> StreamHandler::readBufferDecompress(std::unordered_map<char, std::string>& codes) {
	
	// read the map
	streamInterface->readMap(codes);

	// read the data size
	int dataSize;
	streamInterface->readData(dataSize);

	// read the data
	int bufferSize = (dataSize + 7) / 8;
	std::vector<char> dataBuffer(bufferSize);
	streamInterface->readData(dataBuffer, dataSize);

	// return the value
	std::vector<char> binaryBuffer = convertToBinaryVector(dataBuffer);
	for (int i = 0; i < bufferSize * 8 - dataSize; i++)
		binaryBuffer.pop_back();
	return binaryBuffer;
}

//write to file the decompressed buffer
void StreamHandler::writeBufferDecompress(const std::vector<char>& text) {
	
	// Write the vector content as a single string to the file
	streamInterface->writeData(text);
}

//insert password
void StreamHandler::insertPassword(const std::string& password) {
	std::vector<char> buffer(password.begin(), password.end());
	streamInterface->writeData(buffer);
}

//insert file extension
void StreamHandler::insertFileExtension(const std::string& fileName) {
	// Extract the file extension
	int pos = fileName.find_last_of('.');
	if (pos == std::string::npos) {
		Logger::logError(Logger::NO_EXTENSION_FOUND);
		exit(1);
	}
	std::string extension = fileName.substr(pos);
	std::vector<char> buffer(extension.begin(), extension.end());
	int extensionSize = buffer.size();

	// write the extension size
	streamInterface->writeData(extensionSize);

	// write the extension
	streamInterface->writeData(buffer);
}

//get remaining size to read
int StreamHandler::getRemainingBytesToRead() {
	return streamInterface->getRemainingBytesToRead();
}

//check if the password is correct to the decompressed file's password
bool StreamHandler::isCorrectPassword(const std::string& password) {

	std::vector<char> passwordVector(password.size());
	streamInterface->readData(passwordVector, password.size());
	if (passwordVector.empty()) {
		Logger::logError(Logger::FAILED_READ_PASSWORD_FROM_FILE);
		exit(1);
	}
	std::string passwordString(passwordVector.begin(), passwordVector.end());
	return passwordString == password;
}

void StreamHandler::setMaxWindowSize() {
	long long fileSize = streamInterface->getSourceSize();
	int windowSize = 1024;
	if (fileSize <= 1 * 1024 * 1024) // עד 1MB
		windowSize = 64 * 1024;  // 64KB
	else if (fileSize <= 10 * 1024 * 1024) // עד 10MB
		windowSize = 32 * 1024; // 32KB
	else if (fileSize <= 100 * 1024 * 1024)  // עד 100MB
		windowSize = 16 * 1024; // 16KB
	else if (fileSize <= 1 * 1024 * 1024 * 1024)  // עד 1GB
		windowSize = 8 * 1024;  // 8KB
	LZ77::maxWindowSize = windowSize;
}

