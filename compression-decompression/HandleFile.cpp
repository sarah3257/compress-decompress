#include "HandleFile.h"

HandleFile::HandleFile(const std::string& sourceFilePath, const std::string& destinationFilePath) {
	sourceFile.open(sourceFilePath, std::ios::binary);
	if (!sourceFile) {
		std::cerr << "Error opening file: " << sourceFilePath << std::endl;
	}
	destinationFile.open(destinationFilePath, std::ios::binary);
	if (!sourceFile) {
		std::cerr << "Error opening file: " << destinationFilePath << std::endl;
	}
}

std::vector<char> HandleFile::readBuffer(const std::string& filePath) {

}

void HandleFile::writeBuffer(std::unordered_map<char, std::string>codes, std::string text) {

}
//// read the data in buffers
//while (file.read(buffer.data(), buffer.size()) || inputFile.gcount() > 0) {
//	std::streamsize bytesRead = inputFile.gcount();

//	// מבצעים פעולה כלשהי עם הנתונים מהקובץ
//	for (std::streamsize i = 0; i < bytesRead; ++i) {
//		// כאן אפשר לבצע פעולות עם הנתונים מהקובץ
//		std::cout << buffer[i]; // לדוגמה: הדפסה למסך
//	}
//}

