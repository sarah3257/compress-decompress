#pragma once
#include <string>

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
	static const std::string INVALID_PASSWORD;
	static const std::string CANNOT_SEEK_IN_SOURCE_FILE;
	static const std::string FAILED_READ_4_BYTES_FROM_FILE;
	static const std::string FAILED_READ_KEY_FROM_FILE;
	static const std::string FAILED_READ_VALUE_SIZE_FROM_FILE;
	static const std::string FAILED_READ_DATA_SIZE_FROM_FILE;
	static const std::string FAILED_READ_VALUE_FROM_FILE;
	static const std::string FAILED_READ_DATA_FROM_FILE;
	static const std::string FAILED_READ_PASSWORD_FROM_FILE;
	static const std::string FAILED_READ_EXTENSION_FROM_FILE;
	static const std::string FAILED_READ_EXTENSION_SIZE_FROM_FILE;
	static const std::string FAILED_WRITE_TO_FILE;
	static const std::string NO_EXTENSION_FOUND;
	static const std::string NO_FILE_NAME_FOUND;


	static const std::string NO_FOUND_KEY;
	
private:
	std::string logFileName;
	static std::ofstream logFileStream;
};

