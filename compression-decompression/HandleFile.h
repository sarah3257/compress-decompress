#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <bitset>

class HandleFile
{
	std::ifstream sourceFile;
	std::ofstream destinationFile;
public:
	HandleFile(const std::string& sourceFilePath, bool isCompress,int lengthPassword=0);
	~HandleFile();
	std::vector<char> readBufferCompress();
	void writeBufferCompress(std::unordered_map<char, std::string>codes, std::string text);
	std::vector<char> readBufferDecompress(std::unordered_map<char, std::string>& codes);
	void writeBufferDecompress(std::vector<char> text);
	bool getSourceFileEOF();
	bool getDestinationFileEOF();
	void insertPassword(const std::string& password);
	std::vector<char> convertToBinaryVector(const std::vector<char>& dataBuffer);

	int getFileSizeMinusCurrentSize() {
		std::streampos current_pos = sourceFile.tellg();

		// מעבר לסוף הקובץ
		sourceFile.seekg(0, std::ios::end);

		// קבלת המיקום הנוכחי (שהוא גודל הקובץ)
		std::streampos file_size = sourceFile.tellg();

		// חזרה למיקום המקורי
		sourceFile.seekg(current_pos);

		// חישוב גודל הקובץ שנותר לקרוא
		std::streamsize remaining_size = file_size - current_pos;

		// המרה ל-int אם נחוץ
		int remaining_size_int = static_cast<int>(remaining_size);
		return remaining_size_int;
	}
	void printBinFile() {
		sourceFile.seekg(0, std::ios::end);
		std::streampos fileSize = sourceFile.tellg();
		sourceFile.seekg(0, std::ios::beg);

		std::vector<char> res(fileSize);
		sourceFile.read(res.data(), fileSize);
		sourceFile.close();
		std::cout << "Read " << res.size() << " bytes from file." << std::endl;
		std::cout << "Contents of res: ";
		for (char c : res) {
			std::cout << std::bitset<8>(c) << ' ';
		}
		std::cout << std::endl;
	}
};
