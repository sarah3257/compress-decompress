#pragma once
#include "IStreamInterface.h"
#include <fstream>

class FileStream :public IStreamInterface
{
	std::ifstream sourceFile;
	std::ofstream destinationFile;

public:
	FileStream(const std::string& inputFile, bool isCompress);
	~FileStream();
	void readData(std::vector<char>& buffer, long long size) override;
	void readData(int& size)override;
	void writeData(const std::vector<char>& buffer) override;
	void writeData(int& size) override;
	void writeMap(const std::unordered_map<char, std::string>& codes) override;
	void readMap(std::unordered_map<char, std::string>& codes) override;
	int getRemainingBytesToRead()override;
	long long getSourceSize()override;
	std::string readFileExtension();
	std::string readFileName(const std::string& fileName);

};

