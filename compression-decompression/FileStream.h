#pragma once
#include "IStreamInterface.h"
#include <fstream>

class FileStream :public IStreamInterface
{
	std::ifstream sourceFile;
	std::ofstream destinationFile;

public:
	static double originalFileSize;
	static double destinationFileSize;
	FileStream(const std::string& inputFile);
	~FileStream()override;
	void readData(std::vector<char>& buffer) override;
	void readData(int& size)override;
	void writeData(const std::vector<char>& buffer) override;
	void writeData(int& size) override;
	void writeMap(const std::unordered_map<char, std::string>& codes) override;
	void readMap(std::unordered_map<char, std::string>& codes) override;
	void openDestinationStream(const std::string& sourceNamae, bool isCompress) override;
	int getRemainingBytesToRead() override;
	long long getSourceSize() override;
	std::string readFileExtension();
	std::string readFileName(const std::string& fileName);

};

