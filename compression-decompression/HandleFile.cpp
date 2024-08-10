#include "HandleFile.h"
#include "ErrorHandle.h"
#include "LZ77.h"
#include "IStreamInterface.h"
#include <algorithm>
#include <cmath>
#include <bitset>

//open the source and destination file
HandleFile::HandleFile(IStreamInterface * streamInterface, int lengthPassword):streamInterface(streamInterface){
	
	// read the length of the password
	std::vector<char> passwordBuffer(lengthPassword);
	streamInterface->readData(passwordBuffer, lengthPassword);
	if (passwordBuffer.empty())
		ErrorHandle::handleError(ErrorHandle::CANNOT_SEEK_IN_SOURCE_FILE);
	setMaxWindowSize();
}

//read buffer from file to compress
std::vector<char> HandleFile::readBufferCompress() {

	int remainingSize = getRemainingBytesToRead();
	int size_buffer = std::min(remainingSize, BUFFER_SIZE);
	std::vector<char> bufferText(size_buffer);
	streamInterface->readData(bufferText, size_buffer);
	return bufferText;
}

//write to file the compressed buffer
void HandleFile::writeBufferCompress(const std::unordered_map<char, std::string>& codes, std::string& text) {
	
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
std::vector<char> HandleFile::convertToBinaryVector(const std::vector<char>& dataBuffer) {
	std::vector<char> binaryBuffer;
	for (char ch : dataBuffer) {
		std::bitset<8> binary(ch);
		for (std::size_t i = 0; i < 8; ++i)
			binaryBuffer.push_back(binary.test(7 - i) ? '1' : '0');
	}
	return binaryBuffer;
}

//read buffer from file to decompress and fill the codesMap and data
std::vector<char> HandleFile::readBufferDecompress(std::unordered_map<char, std::string>& codes) {
	
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
void HandleFile::writeBufferDecompress(const std::vector<char>& text) {
	
	// Write the vector content as a single string to the file
	streamInterface->writeData(text);
}

//insert password
void HandleFile::insertPassword(const std::string& password) {
	std::vector<char> buffer(password.begin(), password.end());
	streamInterface->writeData(buffer);
}

//insert file extension
void HandleFile::insertFileExtension(const std::string& fileName) {
	// Extract the file extension
	int pos = fileName.find_last_of('.');
	if (pos == std::string::npos) {
		ErrorHandle::handleError(ErrorHandle::NO_EXTENSION_FOUND);
		exit(1);
	}
	std::string extension = fileName.substr(pos);
	int extensionSize = extension.size();

	// write the extension size
	streamInterface->writeData(extensionSize);

	// write the extension
	std::vector<char> buffer(extension.begin(), extension.end());
	streamInterface->writeData(buffer);
}

//get remaining size to read
int HandleFile::getRemainingBytesToRead() {
	return streamInterface->getRemainingBytesToRead();
}

//check if the password is correct to the decompressed file's password
bool HandleFile::isCorrectPassword(const std::string& text, const std::string& password) {
	// open the file
	std::ifstream sourceFile(text, std::ios::binary);
	if (!sourceFile.is_open()) {
		ErrorHandle::handleError(ErrorHandle::CANNOT_OPEN_FILE);
		exit(1);
	}
	std::string readPassword(password.size(), '\0');
	// read the password
	sourceFile.read(&readPassword[0], password.size());
	if (sourceFile.gcount() != password.size()) {
		sourceFile.close();
		ErrorHandle::handleError(ErrorHandle::FAILED_READ_PASSWORD_FROM_FILE);
		exit(1);
	}
	sourceFile.close();
	return readPassword == password;
}

void HandleFile::setMaxWindowSize() {
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

