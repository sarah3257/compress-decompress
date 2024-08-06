#include "HandleFile.h"
#include <algorithm>
#include <cmath>
#include <bitset>

HandleFile::HandleFile(const std::string& sourceFilePath, bool isCompress, int lengthPassword) {

	sourceFile.open(sourceFilePath, std::ios::binary);
	if (!sourceFile) {
		std::cerr << "Error opening file: " << sourceFilePath << std::endl;
	}

	// move the seekg in the length of the password
	std::streampos offset = lengthPassword;
	sourceFile.seekg(offset, std::ios::beg);
	if (!sourceFile) {
		std::cerr << "Error seeking in source file." << std::endl;
	}

	std::string destinationFilePath = readFileName(sourceFilePath);
	std::string zipExtension = "(zip)";
	if (isCompress)
		destinationFilePath += zipExtension + ".bin";
	else {
		destinationFilePath = destinationFilePath.substr(0, destinationFilePath.size() - zipExtension.size());
		destinationFilePath += "(1)" + readFileExtension();
	}
	destinationFile.open(destinationFilePath, std::ios::binary);
	if (!destinationFile) {
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
	int remainingSize = getFileSizeMinusCurrentSize();
	int size_buffer = std::min(remainingSize, 1024 * 1024);
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

	// read the size of the map
	sourceFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

	if (sourceFile.gcount() != sizeof(mapSize)) {
		throw std::runtime_error("Failed to read 4 bytes from file.");
	}
	int sss = getFileSizeMinusCurrentSize();
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
		int sss = getFileSizeMinusCurrentSize();
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
	int bufferSize = (dataSize + 7) / 8;
	std::vector<char> dataBuffer(bufferSize);
	if (!sourceFile.read(dataBuffer.data(), bufferSize))
		std::cerr << "Failed to read file." << std::endl;

	// return the value
	std::vector<char> binaryBuffer = convertToBinaryVector(dataBuffer);
	for (int i = 0; i < bufferSize * 8 - dataSize; i++)
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
void HandleFile::insertPassword(const std::string& password) {

	if (!destinationFile) {
		throw std::runtime_error("Destination file is not open.");
	}

	destinationFile.write(password.c_str(), password.size());
	if (destinationFile.fail()) {
		throw std::runtime_error("Failed to write to file.");
	}
}

void HandleFile::insertFileExtension(const std::string& fileName) {

	if (!destinationFile.is_open()) {
		throw std::runtime_error("Destination file is not open.");
	}

	// Extract the file extension
	int pos = fileName.find_last_of('.');
	if (pos == std::string::npos) {
		std::cerr << "No extension found." << std::endl;
	}
	std::string extension = fileName.substr(pos);

	// Get the size of the extension
	int extensionSize = extension.size();

	// Write the size of the extension to the file
	destinationFile.write(reinterpret_cast<const char*>(&extensionSize), sizeof(int));
	if (destinationFile.fail()) {
		throw std::runtime_error("Failed to write extension size to file.");
	}

	// Write the extension itself to the file
	destinationFile.write(extension.c_str(), extension.size());
	if (destinationFile.fail()) {
		throw std::runtime_error("Failed to write extension to file.");
	}
}
std::string HandleFile::readFileName(const std::string& fileName) {

	int pos = fileName.find_last_of('.');
	if (pos == std::string::npos) {
		std::cerr << "No filr name found." << std::endl;
	}
	return fileName.substr(0, pos);
}
std::string HandleFile::readFileExtension() {

	if (!sourceFile.is_open()) {
		throw std::runtime_error("Source file is not open.");
	}

	// Read the size of the extension from the file
	int extensionSize = 0;
	sourceFile.read(reinterpret_cast<char*>(&extensionSize), sizeof(int));
	if (sourceFile.fail()) {
		throw std::runtime_error("Failed to read extension size from file.");
	}

	// Read the extension itself from the file
	std::string fileExtension(extensionSize, '\0');
	sourceFile.read(&fileExtension[0], extensionSize);
	if (sourceFile.fail()) {
		throw std::runtime_error("Failed to read extension from file.");
	}
	// Return the file extension
	return fileExtension;
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


