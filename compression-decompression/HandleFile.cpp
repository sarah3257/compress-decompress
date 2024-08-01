#include "HandleFile.h"

HandleFile::HandleFile(const std::string& sourceFilePath, bool isCompress) {
	sourceFile.open(sourceFilePath, std::ios::binary);
	if (!sourceFile) {
		std::cerr << "Error opening file: " << sourceFilePath << std::endl;
	}
	destinationFile.open(destinationFilePath, std::ios::binary);
	if (!sourceFile) {
		std::cerr << "Error opening file: " << destinationFilePath << std::endl;
	}
}

std::vector<char> HandleFile::readBuffer() {

}
std::vector<char> HandleFile::readBufferCompress() {
	std::vector<char>result;
	//fill!!!!!!!
	return result;
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
	std::vector<char> buffer;
	for (int i = 0; i < text.size(); i += 8) {
		std::string byteString = text.substr(i, 8);
		std::bitset<8> byte(byteString);
		buffer.push_back(static_cast<char>(byte.to_ulong()));
	}
	int dataSize = buffer.size();
	destinationFile.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
	destinationFile.write(buffer.data(), buffer.size());
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

