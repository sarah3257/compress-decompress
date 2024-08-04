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
	HandleFile(const std::string& sourceFilePath,bool isCompress);
	~HandleFile();
	std::vector<char> readBufferCompress();
	void writeBufferCompress(std::unordered_map<char, std::string>codes, std::string text);
	std::vector<char> readBufferDecompress(std::unordered_map<char, std::string>& codes);
	void writeBufferDecompress(std::vector<char> text);
	bool getSourceFileEOF();
	bool getDestinationFileEOF();
	void insertPassword(const char* password);
	int getFileSizeMinusCurrentSize() {
		std::streampos current_pos = sourceFile.tellg();

		// ���� ���� �����
		sourceFile.seekg(0, std::ios::end);

		// ���� ������ ������ (���� ���� �����)
		std::streampos file_size = sourceFile.tellg();

		// ���� ������ ������
		sourceFile.seekg(current_pos);

		// ����� ���� ����� ����� �����
		std::streamsize remaining_size = file_size - current_pos;

		// ���� �-int �� ����
		int remaining_size_int = static_cast<int>(remaining_size);
		return remaining_size_int;
	}
};
