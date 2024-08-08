#include "HandleFile.h"
#include "ErrorHandle.h"
#include <algorithm>
#include <cmath>
#include <bitset>

//open the source and destination file
HandleFile::HandleFile(const std::string& sourceFilePath, bool isCompress, int lengthPassword) {
	sourceFile.open(sourceFilePath, std::ios::binary);
	if (!sourceFile)
		ErrorHandle::handleError(ErrorHandle::CANNOT_OPEN_FILE);
	std::streampos offset = lengthPassword;
	sourceFile.seekg(offset, std::ios::beg);
	if (!sourceFile)
		ErrorHandle::handleError(ErrorHandle::CANNOT_SEEK_IN_SOURCE_FILE);
	std::string destinationFilePath = readFileName(sourceFilePath);
	std::string zipExtension = "(zip)";
	if (isCompress)
		destinationFilePath += zipExtension + ".bin";
	else {
		destinationFilePath = destinationFilePath.substr(0, destinationFilePath.size() - zipExtension.size());
		destinationFilePath += "(1)" + readFileExtension();
	}
	destinationFile.open(destinationFilePath, std::ios::binary);
	if (!destinationFile)
		ErrorHandle::handleError(ErrorHandle::CANNOT_OPEN_FILE);
}

//close open files
HandleFile::~HandleFile() {
	if (sourceFile.is_open())
		sourceFile.close();
	if (destinationFile.is_open())
		destinationFile.close();
}

//read buffer from file to compress
std::vector<char> HandleFile::readBufferCompress() {
	int remainingSize = getRemainingBytesToRead();
	int size_buffer = std::min(remainingSize, BUFFER_SIZE);
	if (!sourceFile) {
		ErrorHandle::handleError(ErrorHandle::CANNOT_OPEN_FILE);
		exit(1);
	}
	std::vector<char> bufferText(size_buffer);
	sourceFile.read(bufferText.data(), size_buffer);
	return bufferText;
}

//write to file the compressed buffer
void HandleFile::writeBufferCompress(const std::unordered_map<char, std::string>& codes, std::string& text) {
	//push map.size and map
	int mapSize = codes.size();
	destinationFile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));
	for (const auto& pair : codes) {
		destinationFile.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));
		int strSize = pair.second.size();
		destinationFile.write(reinterpret_cast<const char*>(&strSize), sizeof(strSize));
		destinationFile.write(pair.second.c_str(), strSize);
	}
	//push data.size and date
	int dataSize = text.size();
	while (text.size() % 8)
		text.push_back('0');
	std::vector<char> buffer;
	for (int i = 0; i < text.size(); i += 8) {
		std::string byteString = text.substr(i, 8);
		std::bitset<8> byte(byteString);
		buffer.push_back(static_cast<char>(byte.to_ulong()));
	}
	destinationFile.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
	destinationFile.write(buffer.data(), buffer.size());
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
	int mapSize;
	int valueSize;
	char key;
	// read the size of the map
	sourceFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
	if (sourceFile.gcount() != sizeof(mapSize)) {
		ErrorHandle::handleError(ErrorHandle::FAILED_READ_4_BYTES_FROM_FILE);
		exit(1);
	}
	// read the map from the file to an unordered_map
	for (int i = 0; i < mapSize; ++i) {
		// read the key
		sourceFile.read(reinterpret_cast<char*>(&key), sizeof(key));
		if (sourceFile.gcount() != sizeof(key)) {
			ErrorHandle::handleError(ErrorHandle::FAILED_READ_KEY_FROM_FILE);
			exit(1);
		}
		// read the value size
		sourceFile.read(reinterpret_cast<char*>(&valueSize), sizeof(valueSize));
		if (sourceFile.gcount() != sizeof(valueSize)) {
			ErrorHandle::handleError(ErrorHandle::FAILED_READ_VALUE_SIZE_FROM_FILE);
			exit(1);
		}
		// read the value
		std::string value(valueSize, '\0');
		sourceFile.read(&value[0], valueSize);
		if (sourceFile.gcount() != valueSize) {
			ErrorHandle::handleError(ErrorHandle::FAILED_READ_VALUE_FROM_FILE);
			exit(1);
		}
		// insert the value to the unordered_map
		codes[key] = value;
	}
	// read the data size
	int dataSize;
	sourceFile.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
	if (sourceFile.gcount() != sizeof(dataSize)) {
		ErrorHandle::handleError(ErrorHandle::FAILED_READ_DATA_SIZE_FROM_FILE);
		exit(1);
	}
	// read the data
	int bufferSize = (dataSize + 7) / 8;
	std::vector<char> dataBuffer(bufferSize);
	if (!sourceFile.read(dataBuffer.data(), bufferSize)) {
		ErrorHandle::handleError(ErrorHandle::FAILED_READ_DATA_FROM_FILE);
		exit(1);
	}
	// return the value
	std::vector<char> binaryBuffer = convertToBinaryVector(dataBuffer);
	for (int i = 0; i < bufferSize * 8 - dataSize; i++)
		binaryBuffer.pop_back();
	return binaryBuffer;
}

//write to file the decompressed buffer
void HandleFile::writeBufferDecompress(const std::vector<char>& text) {
	if (!destinationFile) {
		ErrorHandle::handleError(ErrorHandle::CANNOT_OPEN_FILE);
		exit(1);
	}
	// Write the vector content as a single string to the file
	destinationFile.write(text.data(), text.size());
}

//insert password
void HandleFile::insertPassword(const std::string& password) {
	if (!destinationFile) {
		ErrorHandle::handleError(ErrorHandle::CANNOT_OPEN_FILE);
		exit(1);
	}
	destinationFile.write(password.c_str(), password.size());
	if (destinationFile.fail()) {
		ErrorHandle::handleError(ErrorHandle::FAILED_WRITE_TO_FILE);
		exit(1);
	}
}

//insert file extension
void HandleFile::insertFileExtension(const std::string& fileName) {
	if (!destinationFile.is_open()) {
		ErrorHandle::handleError(ErrorHandle::CANNOT_OPEN_FILE);
		exit(1);
	}
	// Extract the file extension
	int pos = fileName.find_last_of('.');
	if (pos == std::string::npos) {
		ErrorHandle::handleError(ErrorHandle::NO_EXTENSION_FOUND);
		exit(1);
	}
	std::string extension = fileName.substr(pos);
	int extensionSize = extension.size();
	destinationFile.write(reinterpret_cast<const char*>(&extensionSize), sizeof(int));
	if (destinationFile.fail()) {
		ErrorHandle::handleError(ErrorHandle::FAILED_WRITE_TO_FILE);
		exit(1);
	}
	destinationFile.write(extension.c_str(), extension.size());
	if (destinationFile.fail()) {
		ErrorHandle::handleError(ErrorHandle::FAILED_WRITE_TO_FILE);
		exit(1);
	}
}

//read file name
std::string HandleFile::readFileName(const std::string& fileName) {
	int pos = fileName.find_last_of('.');
	if (pos == std::string::npos) {
		ErrorHandle::handleError(ErrorHandle::NO_FILE_NAME_FOUND);
		exit(1);
	}
	return fileName.substr(0, pos);
}

//read file extension
std::string HandleFile::readFileExtension() {
	if (!sourceFile.is_open()) {
		ErrorHandle::handleError(ErrorHandle::CANNOT_OPEN_FILE);
		exit(1);
	}
	// Read the size of the extension from the file
	int extensionSize = 0;
	sourceFile.read(reinterpret_cast<char*>(&extensionSize), sizeof(int));
	if (sourceFile.fail()) {
		ErrorHandle::handleError(ErrorHandle::FAILED_READ_EXTENSION_SIZE_FROM_FILE);
		exit(1);
	}
	// Read the extension itself from the file
	std::string fileExtension(extensionSize, '\0');
	sourceFile.read(&fileExtension[0], extensionSize);
	if (sourceFile.fail()) {
		ErrorHandle::handleError(ErrorHandle::FAILED_READ_EXTENSION_FROM_FILE);
		exit(1);
	}
	return fileExtension;
}

//get remaining size to read
int HandleFile::getRemainingBytesToRead() {
	std::streampos current_pos = sourceFile.tellg();
	sourceFile.seekg(0, std::ios::end);
	std::streampos file_size = sourceFile.tellg();
	sourceFile.seekg(current_pos);
	int remaining_size = file_size - current_pos;
	return remaining_size;
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

