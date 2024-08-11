#include "Logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <windows.h>

//open logFile
Logger::Logger(const std::string& logFileName)
{
	logFileStream.open(logFileName, std::ios::app);
	if (!logFileStream)
		throw std::runtime_error("Failed to open log file");
    testFileStream.open("test.txt", std::ios::app);
    if (!testFileStream)
        throw std::runtime_error("Failed to open test file");
}

//close logFile
Logger::~Logger()
{
	if (logFileStream.is_open())
		logFileStream.close();
    if (testFileStream.is_open())
        testFileStream.close();

}
 std::wstring Logger::stringToWstring(const std::string& str) {
    return std::wstring(str.begin(), str.end());
}

//Information handle
void Logger::logInfo(const std::string& message)
{
    log("Information", message);
}


//Warning handle 
void Logger::logWarning(const std::string& message)
{
    log("Warning", message);
    MessageBox(NULL, stringToWstring(message).c_str(), L"Warning", MB_YESNO | MB_ICONQUESTION);

}

//Error handle
void Logger::logError(const std::string& message)
{
    log("Error", message);
    MessageBox(NULL, stringToWstring(message).c_str(), L"Error", MB_YESNO | MB_ICONERROR);
    exit(1);
}

void Logger::logTest(const std::string& message)
{
    log("Test", message);

}

// Write to log file
void Logger::log(const std::string& level, const std::string& message)
{
    // Gets the current time
    std::time_t now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now);
    if (level == "Test") {
        if(testFileStream.is_open()){
        testFileStream << "[" << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << "] "
            << level << ": " << message << std::endl;
        }
    }
    else if (logFileStream.is_open()) {
        logFileStream << "[" << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << "] "
            << level << ": " << message << std::endl;
    }
    else {
        // if file is not opened, the message will be displayed in MessageBox
        MessageBox(NULL, L"Failed to write to log file ", L"Failed Logger", MB_YESNO | MB_ICONQUESTION);
    }
}


//static information messages
const std::string Logger::START_FUNCTION = "start Function: ";
const std::string Logger::END_FUNCTION = "Function exit";
const std::string Logger::IN_CLASS = "In class: ";
const std::string Logger::TEST_EMPTY_FILE = " The system supports empty files ";
const std::string Logger::TEST_RANDOM_FILE = " The system supports random files ";

//static Warning messages
const std::string Logger:: WARNING_LARGE_FILE="Compressing a large file takes a long time";


// Definition of static const error messages
const std::string Logger::CANNOT_OPEN_FILE = "Cannot open the file.";
const std::string Logger::TRYING_TO_DECODE_UNCOMPRESSED_FILE = "Trying to decode an uncompressed file.";
const std::string Logger::NO_BUFFER_CHARACTER_FOUND = "No buffer character found.";
const std::string Logger::INVALID_PASSWORD = "Invalid password";
const std::string Logger::CANNOT_SEEK_IN_SOURCE_FILE = "Can't seek in source file";
const std::string Logger::FAILED_READ_4_BYTES_FROM_FILE = "Failed to read 4 bytes from file";
const std::string Logger::FAILED_READ_KEY_FROM_FILE = "Failed read key from file";
const std::string Logger::FAILED_READ_VALUE_SIZE_FROM_FILE = "Failed read value size from file";
const std::string Logger::FAILED_READ_DATA_SIZE_FROM_FILE = "Failed read data size from file";
const std::string Logger::FAILED_READ_VALUE_FROM_FILE = "Failed read value from file";
const std::string Logger::FAILED_READ_DATA_FROM_FILE = "Failed read data from file";
const std::string Logger::FAILED_READ_PASSWORD_FROM_FILE = "Failed read password from file";
const std::string Logger::FAILED_READ_EXTENSION_FROM_FILE = "Failed read extension from file";
const std::string Logger::FAILED_READ_EXTENSION_SIZE_FROM_FILE = "Failed read extension from file";
const std::string Logger::FAILED_WRITE_TO_FILE = "Failed write to file";
const std::string Logger::NO_EXTENSION_FOUND = "No extension found";
const std::string Logger::NO_FILE_NAME_FOUND = "No file name found";
const std::string Logger::NO_FOUND_KEY = "No found key.";

std::ofstream Logger::logFileStream;
std::ofstream Logger::testFileStream;
