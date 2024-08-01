#include "HandleFile.h"
#include <algorithm>

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
		if(isTxt)
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
	if (sourceFile.is_open()) {
		sourceFile.close();
	}
	if (destinationFile.is_open()) {
		destinationFile.close();
	}
}
//read buffer &  write vec<char>
std::vector<char> HandleFile::readBufferCompress() {
	int size_buffer= 1024 * 1024;
	if (!sourceFile) {
		std::cerr << "Error opening file for reading: "  << std::endl;
		return {};
	}
	std::vector<char> bufferText(size_buffer);

	sourceFile.read(bufferText.data(), size_buffer);

	return bufferText;

}


void HandleFile::writeBufferCompress(std::unordered_map<char, std::string>codes, std::string text) {

}
std::vector<char> HandleFile::readBufferDecompress() {
	std::vector<char>result;
	//fill!!!!!!!
	return result;
}
void HandleFile::writeBufferDecompress(std::vector<char> text) {

}
bool HandleFile::getSourceFileEOF() {
	return sourceFile.eof();
}
bool HandleFile::getDestinationFileEOF() {
	return destinationFile.eof();
}

