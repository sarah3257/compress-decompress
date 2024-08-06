#include "ErrorHandle.h"

// Constructor to open the log file
ErrorHandle::ErrorHandle(const std::string& logFile) : logFileName(logFile) {
	logFileStream.open(logFileName, std::ios::app);
	if (!logFileStream)
		throw std::runtime_error("Failed to open log file");
}

// Destructor to close the log file
ErrorHandle::~ErrorHandle() {
	if (logFileStream.is_open())
		logFileStream.close();
}

// Function to handle and log errors
void ErrorHandle::handleError(const std::string& errorMessage) {
	// Print error to console
	std::cerr << errorMessage << std::endl;
	// Write error to log file
	if (logFileStream.is_open())
		logFileStream << errorMessage << std::endl;
	else
		std::cerr << "Failed to write to log file: " << errorMessage << std::endl;
}

// Definition of static const error messages
const std::string ErrorHandle::CANNOT_OPEN_FILE = "Cannot open the file.";
const std::string ErrorHandle::TRYING_TO_DECODE_UNCOMPRESSED_FILE = "Trying to decode an uncompressed file.";
const std::string ErrorHandle::NO_BUFFER_CHARACTER_FOUND = "No buffer character found.";
const std::string ErrorHandle::INVALID_PASSWORD = "Invalid password";
const std::string ErrorHandle::CANNOT_SEEK_IN_SOURCE_FILE = "Can't seek in source file";
const std::string ErrorHandle::FAILED_READ_4_BYTES_FROM_FILE = "Failed to read 4 bytes from file";
const std::string ErrorHandle::FAILED_READ_KEY_FROM_FILE = "Failed read key from file";
const std::string ErrorHandle::FAILED_READ_VALUE_SIZE_FROM_FILE = "Failed read value size from file";
const std::string ErrorHandle::FAILED_READ_DATA_SIZE_FROM_FILE = "Failed read data size from file";
const std::string ErrorHandle::FAILED_READ_VALUE_FROM_FILE = "Failed read value from file";
const std::string ErrorHandle::FAILED_READ_DATA_FROM_FILE = "Failed read data from file";
const std::string ErrorHandle::FAILED_READ_PASSWORD_FROM_FILE = "Failed read password from file";
const std::string ErrorHandle::FAILED_READ_EXTENSION_FROM_FILE = "Failed read extension from file";
const std::string ErrorHandle::FAILED_READ_EXTENSION_SIZE_FROM_FILE = "Failed read extension from file";
const std::string ErrorHandle::FAILED_WRITE_TO_FILE = "Failed write to file";
const std::string ErrorHandle::NO_EXTENSION_FOUND = "No extension found";
const std::string ErrorHandle::NO_FILE_NAME_FOUND = "No file name found";


std::ofstream ErrorHandle::logFileStream;