#include "HandleFile.h"
#include <algorithm>
#include <cmath>
#include <bitset>


HandleFile::HandleFile(const std::string& sourceFilePath, bool isCompress) {
	//bool typeFile = true;//we need do it.
	sourceFile.open(sourceFilePath, std::ios::binary);
	if (!sourceFile) {
		std::cerr << "Error opening file: " << sourceFilePath << std::endl;
	}
	std::string destinationFilePath;
	if (isCompress)
		destinationFilePath = sourceFilePath.substr(0, sourceFilePath.size() - 4) + "STZ_COMPRESS.bin";
	else {
		char isTxt = true;
		//sourceFile.read(&isTxt, sizeof(isTxt));
		if (isTxt)
			destinationFilePath = sourceFilePath.substr(0, sourceFilePath.size() - 12) + ".txt";
		else
			destinationFilePath = sourceFilePath.substr(0, sourceFilePath.size() - 15) + "11.bin";
	}
	destinationFile.open(destinationFilePath, std::ios::binary);
	if (!sourceFile) {
		std::cerr << "Error opening file: " << destinationFilePath << std::endl;
	}
}
HandleFile::~HandleFile() {
	if (sourceFile.is_open()) {
		sourceFile.close();
	}
	if (destinationFile.is_open()) {
		destinationFile.close();
	}
}
//read buffer &  write vec<char>
std::vector<char> HandleFile::readBufferCompress() {
	std::streampos current_pos = sourceFile.tellg();
	sourceFile.seekg(0, std::ios::end);
	int file_size = sourceFile.tellg();
	sourceFile.seekg(current_pos);
	int size_buffer = std::min(file_size, 1024 * 1024);
	if (!sourceFile) {
		std::cerr << "Error opening file for reading: " << std::endl;
		return {};
	}
	std::vector<char> bufferText(size_buffer);

	sourceFile.read(bufferText.data(), size_buffer);

	return bufferText;


}


void HandleFile::writeBufferCompress(std::unordered_map<char, std::string>codes, std::string text) {
	//push map.size and map
	/*int mapSize = codes.size();
	destinationFile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));
	for (const auto& pair : codes) {
		destinationFile.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));
		int strSize = pair.second.size();
		destinationFile.write(reinterpret_cast<const char*>(&strSize), sizeof(strSize));
		destinationFile.write(pair.second.c_str(), strSize);
	}*/
	//push data.size and date
	std::vector<char> buffer;
	for (int i = 0; i < text.size(); i += 8) {
		std::string byteString = text.substr(i, 8);
		std::bitset<8> byte(byteString);
		buffer.push_back(static_cast<char>(byte.to_ulong()));
	}
	int dataSize = text.size();
	destinationFile.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
	destinationFile.write(buffer.data(), buffer.size());
}



std::vector<char> convertToBinaryVector(const std::vector<char>& dataBuffer) {
	std::vector<char> binaryBuffer;

	for (char ch : dataBuffer) {
		std::bitset<8> binary(ch);
		for (std::size_t i = 0; i < 8; ++i) {
			binaryBuffer.push_back(binary.test(7 - i) ? '1' : '0'); // add binary representation of each character
		}
	}

	return binaryBuffer;
}
std::vector<char> HandleFile::readBufferDecompress(std::unordered_map<char, std::string>& codes) {
	int mapSize;
	int valueSize;
	char key;
	printBinFile();
	// read the size of the map
	sourceFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
	if (sourceFile.gcount() != sizeof(mapSize)) {
		throw std::runtime_error("Failed to read 4 bytes from file.");
	}

	// read the map from the file to an unordered_map
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
	int bufferSize = (dataSize + 8) / 8;
	std::vector<char> dataBuffer(bufferSize);
	//std::string binaryString = "";

	if (!sourceFile.read(dataBuffer.data(), bufferSize))
		std::cerr << "Failed to read file." << std::endl;


	// return the value
	std::vector<char> binaryBuffer = convertToBinaryVector(dataBuffer);
	for (int i = 0; i < bufferSize*8-dataSize; i++)
		binaryBuffer.pop_back();
	return binaryBuffer;
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
	std::streampos current_pos = sourceFile.tellg();
	std::cout << "Current position: " << current_pos << std::endl;
	//
	if (sourceFile.eof()) {
		return true;
	}

    current_pos = sourceFile.tellg();
	sourceFile.seekg(0, std::ios::end);
	std::streampos end_pos = sourceFile.tellg();
	sourceFile.seekg(current_pos);

	return current_pos == end_pos;
}
bool HandleFile::getDestinationFileEOF() {
	if (destinationFile.eof()) {
		return true;
	}

	std::streampos current_pos = destinationFile.tellp();
	destinationFile.seekp(0, std::ios::end);
	std::streampos end_pos = destinationFile.tellp();
	destinationFile.seekp(current_pos);

	return current_pos == end_pos;
}
void HandleFile::insertPassword(const char* password) {

	if (!destinationFile) {
		throw std::runtime_error("Destination file is not open.");
	}
	destinationFile.write(password, strlen(password) + 1);
	if (destinationFile.fail()) {
		throw std::runtime_error("Failed to write to file.");
	}
}
std::vector<char> HandleFile::convertToBinaryVector(const std::vector<char>& dataBuffer) {
	std::vector<char> binaryBuffer;
	for (char ch : dataBuffer) {
		std::bitset<8> binary(ch);
		for (std::size_t i = 0; i < 8; ++i) {
			binaryBuffer.push_back(binary.test(7 - i) ? '1' : '0'); // add binary representation of each character
		}
	}
	return binaryBuffer;
}


