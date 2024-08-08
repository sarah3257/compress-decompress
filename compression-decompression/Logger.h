#pragma once
#include <string>
#include <fstream>
class Logger
{
public:
	Logger(const std::string& logFile);


	~Logger();

	static std::wstring stringToWstring(const std::string& str);
	//static information messages
	static const std::string START_FUNCTION;
	static const std::string IN_CLASS;
	static const std::string END_FUNCTION;
	static const std::string IN_CLASS;

	//static Warning messages
	static const std::string WARNING_LARGE_FILE;

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

	static void logInfo(const std::string& message);
	static void logWarning(const std::string& message);
	static void logError(const std::string& message);
	
	private:
	static std::ofstream logFileStream;
	static void log(const std::string& level, const std::string& message);


};

