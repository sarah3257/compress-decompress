#include "ErrorHandle.h"
#include <iostream>
#include <fstream>

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

std::ofstream ErrorHandle::logFileStream;