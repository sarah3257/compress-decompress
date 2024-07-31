#pragma once
#include <string>
#include <string.h>
#include <fstream>
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
	
private:
	std::string logFileName;
	std::ofstream logFileStream;
};

