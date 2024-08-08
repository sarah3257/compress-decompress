#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

static const int BUFFER_SIZE = 1024 * 1024 * 4;

class HandleFile
{
	std::ifstream sourceFile;
	std::ofstream destinationFile;
public:
	HandleFile(const std::string& sourceFilePath, bool isCompress = true, int lengthPassword = 0);
	~HandleFile();
	std::vector<char> readBufferCompress();
	std::vector<char> readBufferDecompress(std::unordered_map<char, std::string>& codes);
	std::vector<char> convertToBinaryVector(const std::vector<char>& dataBuffer);
	void writeBufferCompress(const std::unordered_map<char, std::string>& codes, std::string& text);
	void writeBufferDecompress(const std::vector<char>& text);
	void insertPassword(const std::string& password);
	void insertFileExtension(const std::string& fileName);
	std::string readFileExtension();
	std::string readFileName(const std::string& fileName);
	int getRemainingBytesToRead();
	long long getSourceFileSize();
	void setMaxWindowSize();

	static bool isCorrectPassword(const std::string& text, const std::string& password);
};
