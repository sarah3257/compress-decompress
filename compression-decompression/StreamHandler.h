#pragma once
#include "IStreamInterface.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

static const int BUFFER_SIZE = 1024 * 1024 * 4;

class StreamHandler
{
	IStreamInterface* streamInterface;

public:
	static std::vector<char> convertToBinaryVector(const std::vector<char>& dataBuffer);
	StreamHandler(IStreamInterface* streamInterfaceh);
	std::vector<char> readBufferCompress();
	std::vector<char> readBufferDecompress(std::unordered_map<char, std::string>& codes);
	void writeBufferCompress(const std::unordered_map<char, std::string>& codes, std::string& text);
	void writeBufferDecompress(const std::vector<char>& text);
	bool isCorrectPassword(const std::string& password) ;
	void insertPassword(const std::string& password);
	void insertFileExtension(const std::string& fileName);
	int getRemainingBytesToRead();
	void setMaxWindowSize();

};
