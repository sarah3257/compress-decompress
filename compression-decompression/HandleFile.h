#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

class HandleFile
{
	


	std::ifstream sourceFile;
	std::ofstream destinationFile;
public:
	HandleFile(const std::string& sourceFilePath,bool isCompress);
	~HandleFile();
	std::vector<char> readBufferCompress();
	void writeBufferCompress(std::unordered_map<char, std::string>codes, std::string text);
	std::vector<char> readBufferDecompress();
	void writeBufferDecompress(std::vector<char> text);
	bool getSourceFileEOF();
	bool getDestinationFileEOF();
	void insertPassword(const char* password);
};
