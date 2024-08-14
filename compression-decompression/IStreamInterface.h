#pragma once
#include <vector>
#include <unordered_map>
#include <string>

class IStreamInterface
{
public:
	virtual ~IStreamInterface();
	virtual void readData(std::vector<char>& buffer) = 0;
	virtual void readData(int& size) = 0;
	virtual void writeData(const std::vector<char>& buffer) = 0;
	virtual void writeData(int& size) = 0;
	virtual void writeMap(const std::unordered_map<char, std::string>& codes) = 0;
	virtual void readMap(std::unordered_map<char, std::string>& codes) = 0;
	virtual void openDestinationStream(const std::string& sourceNamae, bool isCompress)=0;
	virtual int getRemainingBytesToRead() = 0;
	virtual long long getSourceSize() = 0;
};

