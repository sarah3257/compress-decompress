#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
class ErrorHandle
{
public:
	ErrorHandle(const std::string& logFile);
	~ErrorHandle();
	static void handleError(const std::string& errorMessage);
	//static error messages 
	static const std::string CANNOT_OPEN_FILE;
	static const std::string TRYING_TO_DECODE_UNCOMPRESSED_FILE;
	static const std::string NO_BUFFER_CHARACTER_FOUND;
	static const std::string NO_FOUND_KEY;
	
private:
	std::string logFileName;
	static std::ofstream logFileStream;
};

