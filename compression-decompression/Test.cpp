#include "Test.h"
#include "Logger.h"
#include <ctime>
#include <iomanip>
#include <windows.h>


void Test::createEmptyFile(const std::string& filename)
{
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (file.is_open()) {
        file.close();
        std::cout << "Empty file created: " << filename << std::endl;
    }
    else {
        std::cerr << "Failed to create empty file: " << filename << std::endl;
    }
}



bool Test:: areFilesEqual(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1, std::ios::binary);
    std::ifstream f2(file2, std::ios::binary);

    if (!f1.is_open() || !f2.is_open()) {
        std::cerr << "Failed to open one or both files." << std::endl;
        return false;
    }

    std::istreambuf_iterator<char> begin1(f1), end1;
    std::istreambuf_iterator<char> begin2(f2);

    return std::equal(begin1, end1, begin2);
}

void Test::testEmptyFile()
{
    std::string filename = "empty_file.txt";
    createEmptyFile(filename);
    Deflate::compress(filename);
    Deflate::decompress("empty_file(zip).bin");
    if (areFilesEqual(filename, "empty_file(1).txt")) 
        Logger::logInfo(Logger::TEST_EMPTY_FILE);
    else {
        MessageBox(NULL, L"Test error in the testEmptyFile function", L"Error_Test", MB_YESNO | MB_ICONERROR);
        exit(1);
    }


}