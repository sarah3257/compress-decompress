#include "Test.h"
#include "Logger.h"
#include "CompressionDecompression.h"
#include <ctime>
#include <iomanip>
#include <windows.h>
#include <cstdlib>  //  rand & srand


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
    std::string filename = "test/empty_file.txt";
    createEmptyFile(filename);
    CompressionDecompression::compress(filename);
    CompressionDecompression::decompress("test/empty_file(zip).bin");
    if (areFilesEqual(filename, "test/empty_file(1).txt")) 
        Logger::logTest(Logger::TEST_EMPTY_FILE);
    else {
        MessageBox(NULL, L"Test error in the testEmptyFile function", L"Error_Test", MB_YESNO | MB_ICONERROR);
        exit(1);
    }


}

void Test::testRandomFile() {
    writeRandomValuesToTextFile();
    CompressionDecompression::compress("test/randomFile.txt");
    CompressionDecompression::decompress("test/randomFile(zip).bin");
    if (areFilesEqual("test/randomFile.txt", "test/randomFile(1).txt"))
        Logger::logTest(Logger::TEST_RANDOM_FILE);
    else {
        MessageBox(NULL, L"Test error in the testRandomFile function", L"Error_Test", MB_YESNO | MB_ICONERROR);
        exit(1);
    }

}



void Test::writeRandomValuesToTextFile() {
    std::string filename = "test/randomFile.txt";
    std::ofstream outFile(filename);
    if (!outFile) {
        MessageBox(NULL, L"Error opening file for writing:", L"Error_Test", MB_YESNO | MB_ICONERROR);
        return;
    }

    std::srand(std::time(0));
    const int targetSizeKB = 50; // הגודל המבוקש בקילובייטים
    const int targetSizeBytes = targetSizeKB * 1024; // המרה לבייטים
    int currentSizeBytes = 0;

    while (currentSizeBytes < targetSizeBytes) {
        int randomValue = std::rand() % 100; // ערכים רנדומליים בטווח 0-99
        outFile << randomValue << std::endl; // כתיבה לקובץ
        currentSizeBytes += std::to_string(randomValue).size() + 1; // עדכון הגודל הנוכחי של הקובץ
    }

    outFile.close();
}

void Test::writeSmallFile() {
    std::string filename = "test/SmallFile.txt";

    std::ofstream outFile(filename);
    if (!outFile) {
        MessageBox(NULL, L"Error opening file for writing:", L"Error_Test", MB_YESNO | MB_ICONERROR);
        return;
    }

    std::srand(std::time(0));

    const int numChars = 10;
    char repeatedChar = 'a' + std::rand() % 26;  // בחירת תו רנדומלי אחד

    for (int i = 0; i < numChars; ++i) {
        outFile << repeatedChar;  // כתיבת אותו התו מספר פעמים
    }

    outFile.close();
}

void Test::testSmallFile()
{
    writeSmallFile();
    CompressionDecompression::compress("test/SmallFile.txt");
    CompressionDecompression::decompress("test/SmallFile(zip).bin");
    if (areFilesEqual("test/SmallFile.txt", "test/SmallFile(1).txt"))
        Logger::logTest(Logger::TEST_SMALL_FILE);
    else {
        MessageBox(NULL, L"Test error in the testSmallFile function", L"Error_Test", MB_YESNO | MB_ICONERROR);
        exit(1);
    }

}

void Test::playTest()
{
    testEmptyFile();
    testRandomFile();
    testSmallFile();
}