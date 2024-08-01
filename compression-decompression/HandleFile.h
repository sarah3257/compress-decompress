#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

class HandleFile
{
	

public:
	std::ifstream sourceFile;
	std::ofstream destinationFile;

	std::ifstream getSourceFile();
	std::ofstream getDestinationFile();
	HandleFile(const std::string& sourceFilePath);
	std::vector<char> readBuffer();
	void writeBuffer(std::unordered_map<char, std::string>codes, std::string text);
};
