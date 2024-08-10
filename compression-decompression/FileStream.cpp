#include "FileStream.h"
#include "ErrorHandle.h"

FileStream::FileStream(const std::string& sourceFilePath, bool isCompress) {

	sourceFile.open(sourceFilePath, std::ios::binary);
	if (!sourceFile)
		ErrorHandle::handleError(ErrorHandle::CANNOT_OPEN_FILE);
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

FileStream::~FileStream() {
	if (sourceFile.is_open())
		sourceFile.close();
	if (destinationFile.is_open())
		destinationFile.close();
}

void FileStream::readData(std::vector<char>& buffer, long long size_buffer) {
	if (!sourceFile) {
		ErrorHandle::handleError(ErrorHandle::CANNOT_OPEN_FILE);
		exit(1);
	}
	sourceFile.read(buffer.data(), size_buffer);
}


void FileStream::readData(int& dataSize) {
	if (!sourceFile) {
		ErrorHandle::handleError(ErrorHandle::CANNOT_OPEN_FILE);
		exit(1);
	}
	sourceFile.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
	if (sourceFile.gcount() != sizeof(dataSize)) {
		ErrorHandle::handleError(ErrorHandle::FAILED_READ_DATA_SIZE_FROM_FILE);
		exit(1);
	}
}

void FileStream::writeData(const std::vector<char>& buffer) {

	if (!destinationFile) {
		ErrorHandle::handleError(ErrorHandle::CANNOT_OPEN_FILE);
		exit(1);
	}
	// Write the vector content as a single string to the file
	destinationFile.write(buffer.data(), buffer.size());
	if (destinationFile.fail()) {
		ErrorHandle::handleError(ErrorHandle::FAILED_WRITE_TO_FILE);
		exit(1);
	}	
}

void FileStream::writeData(int& size) {
	destinationFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
}

void FileStream::writeMap(const std::unordered_map<char, std::string>& codes) {
	/*int mapSize = codes.size();
	destinationFile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));
	for (const auto& pair : codes) {
		destinationFile.write(&pair.first, sizeof(pair.first));
		int strSize = pair.second.size();
		destinationFile.write(reinterpret_cast<const char*>(&strSize), sizeof(strSize));
		destinationFile.write(pair.second.c_str(), strSize);
	}*/
	//push map.size and map
	int mapSize = codes.size();
	destinationFile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));
	for (const auto& pair : codes) {
		destinationFile.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));
		int strSize = pair.second.size();
		destinationFile.write(reinterpret_cast<const char*>(&strSize), sizeof(strSize));
		destinationFile.write(pair.second.c_str(), strSize);
	}
}

void FileStream::readMap(std::unordered_map<char, std::string>& codes) {
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
}

//V
int FileStream::getRemainingBytesToRead() {
	std::streampos current_pos = sourceFile.tellg();
	sourceFile.seekg(0, std::ios::end);
	std::streampos file_size = sourceFile.tellg();
	sourceFile.seekg(current_pos);
	int remaining_size = file_size - current_pos;
	return remaining_size;
}

//V
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
	if (pos == std::string::npos) {
		ErrorHandle::handleError(ErrorHandle::NO_FILE_NAME_FOUND);
		exit(1);
	}
	return fileName.substr(0, pos);
}

//read file extension
std::string FileStream::readFileExtension() {
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