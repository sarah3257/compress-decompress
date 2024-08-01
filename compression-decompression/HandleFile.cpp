#include "HandleFile.h"
               
HandleFile::HandleFile(const std::string& sourceFilePath, bool isCompress) {
	sourceFile.open(sourceFilePath, std::ios::binary);
	if (!sourceFile) {
		std::cerr << "Error opening file: " << sourceFilePath << std::endl;
	}
	std::string destinationFilePath;
	if (isCompress)
		destinationFilePath = sourceFilePath.substr(0, sourceFilePath.size() - 4) + "STZ_COPRESS.bin";
	else {
		char isTxt;
		sourceFile.read(&isTxt, sizeof(isTxt));
		if (isTxt)
			destinationFilePath = sourceFilePath.substr(0, sourceFilePath.size() - 15) + ".txt";
		else
			destinationFilePath = sourceFilePath.substr(0, sourceFilePath.size() - 15) + ".bin";
	}
	destinationFile.open(destinationFilePath, std::ios::binary);
	if (!sourceFile) {
		std::cerr << "Error opening file: " << destinationFilePath << std::endl;
	}
}
HandleFile::~HandleFile() {

}
std::vector<char> HandleFile::readBufferCompress() {
	std::vector<char>result;
	//fill!!!!!!!
	return result;
}
void HandleFile::writeBufferCompress(std::unordered_map<char, std::string>codes, std::string text) {

}
std::vector<char> HandleFile::readBufferDecompress() {

	int mapSize;
	int valueSize;
	char key;

	// read the size of the map
	sourceFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
	if (sourceFile.gcount() != sizeof(mapSize)) {
		throw std::runtime_error("Failed to read 4 bytes from file.");
	}

	// read the map from the file to an unordered_map
	std::unordered_map<char, std::string> codes;
	for (int i = 0; i < mapSize; ++i) {

		// read the key
		sourceFile.read(reinterpret_cast<char*>(&key), sizeof(key));
		if (sourceFile.gcount() != sizeof(key)) {
			throw std::runtime_error("Failed to read key from file.");
		}

		// read the value size
		sourceFile.read(reinterpret_cast<char*>(&valueSize), sizeof(valueSize));
		if (sourceFile.gcount() != sizeof(valueSize)) {
			throw std::runtime_error("Failed to read value size from file.");
		}

		// read the value
		std::string value(valueSize, '\0');
		sourceFile.read(&value[0], valueSize);
		if (sourceFile.gcount() != valueSize) {
			throw std::runtime_error("Failed to read value from file.");
		}

		// insert the value to the unordered_map
		codes[key] = value;
	}

	// read the data size
	int dataSize;
	sourceFile.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
	if (sourceFile.gcount() != sizeof(dataSize)) {
		throw std::runtime_error("Failed to read data size from file.");
	}

	// read the data
	std::vector<char> dataBuffer(dataSize);

	if (sourceFile.read(dataBuffer.data(), dataSize)) {
		// print the data 
		for (char c : dataBuffer) {
			std::cout << c;
		}
	}
	else {
		std::cerr << "Failed to read file." << std::endl;
	}
	
	// return the value
	return dataBuffer;
}
void HandleFile::writeBufferDecompress(std::vector<char> text) {

	if (!destinationFile) {
		throw std::runtime_error("Destination file is not open.");
	}

	// Write the vector content as a single string to the file
	destinationFile.write(text.data(), text.size());
	if (!destinationFile) {
		throw std::runtime_error("Failed to write to the file.");
	}
}
bool HandleFile::getSourceFileEOF() {
	return sourceFile.eof();
}
bool HandleFile::getDestinationFileEOF() {
	return destinationFile.eof();
}

