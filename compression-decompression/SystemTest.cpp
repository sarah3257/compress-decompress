#include "SystemTest.h"
#include "Logger.h"
#include "CompressionDecompression.h"
#include "Deflate.h"
#include "LZ77.h"
#include <ctime>
#include <iomanip>
#include <windows.h>
#include <cstdlib>  
#include <iostream>
#include <direct.h>
namespace SystemTest
{

	namespace
	{
		bool areFilesEqual(const std::string& file1, const std::string& file2) {
			std::ifstream f1(file1, std::ios::binary);
			std::ifstream f2(file2, std::ios::binary);

			if (!f1.is_open() || !f2.is_open()) {
				return false;
			}

			std::istreambuf_iterator<char> begin1(f1), end1;
			std::istreambuf_iterator<char> begin2(f2);

			return std::equal(begin1, end1, begin2);
		}

		void createRegularFile(const std::string& filename1) {
			const std::string  filename = filename1 + ".txt";
			const std::string text = "It is with great excitement that we present to you our crazy project.\nA lot of energy and effort was invested in calculating the algorithm\n";
			const size_t fileSize = 10 * 1024;
			const size_t textSize = text.size();
			std::ofstream outFile(filename, std::ios::binary);
			if (!outFile) {
				MessageBoxW(NULL, L" error open file", L"Error_Test", MB_YESNO | MB_ICONERROR);
				exit(1);
			}
			size_t repetitions = fileSize / textSize;
			for (size_t i = 0; i < repetitions; ++i) {
				outFile.write(text.c_str(), textSize);
			}
			outFile.close();
		}

		void compressAndDecompress(const std::string& filename)
		{
			CompressionDecompression::compress(filename + ".txt", Deflate::compress);
			CompressionDecompression::decompress(filename + "(" + CompressionDecompression::password + ").bin", Deflate::decompress);
			if (areFilesEqual(filename + ".txt", filename + "(1).txt"))
				Logger::logTest(Logger::TEST_ZERO_FILE);
			else {
				MessageBoxW(NULL, L"An error was found in the test", L"Error_Test", MB_YESNO | MB_ICONERROR);
				exit(1);
			}
		}

		void testRegularFile()
		{
			const std::string filename = "test/normalFile";
			createRegularFile(filename);
			compressAndDecompress(filename);
		}

		void createEmptyFile(const std::string& filename1)
		{
			const std::string  filename = filename1 + ".txt";

			std::ofstream file(filename, std::ios::out | std::ios::trunc);
			if (file.is_open()) {
				file.close();
			}
			else {
				MessageBoxW(NULL, L"Failed to create empty file", L"Error_Test", MB_YESNO | MB_ICONERROR);
				exit(1);
			}
		}

		void testEmptyFile()
		{
			std::string filename = "test/empty_file";
			createEmptyFile(filename);
			compressAndDecompress(filename);
		}

		void writeRandomValuesToTextFile(const std::string& filename1) {
			const std::string  filename = filename1 + ".txt";
			std::ofstream outFile(filename);
			if (!outFile) {
				MessageBoxW(NULL, L"Error opening file for writing:", L"Error_Test", MB_YESNO | MB_ICONERROR);
				return;
			}

			std::srand(static_cast<unsigned int>(std::time(0)));
			const int targetSizeKB = 10;
			const int targetSizeBytes = targetSizeKB * 1024;
			int currentSizeBytes = 0;

			while (currentSizeBytes < targetSizeBytes) {
				int randomValue = std::rand() % 100;
				outFile << randomValue << std::endl;
				currentSizeBytes += static_cast<int>(std::to_string(randomValue).size()) + 1;
			}

			outFile.close();
		}

	

		void testRandomFile() {
			const std::string filename = "test/randomFile";
			writeRandomValuesToTextFile(filename);
			compressAndDecompress(filename);
		}

		void writeSmallFile(const std::string filename1) {
			const std::string  filename = filename1 + ".txt";
			std::ofstream outFile(filename);
			if (!outFile) {
				MessageBoxW(NULL, L"Error opening file for writing:", L"Error_Test", MB_YESNO | MB_ICONERROR);
				return;
			}
			std::srand(static_cast<unsigned int>(std::time(0)));
			const int numChars = 100;
			char repeatedChar = 'a' + std::rand() % 26;

			for (int i = 0; i < numChars; ++i) {
				outFile << repeatedChar;
			}

			outFile.close();
		}

		void testSmallFile()
		{
			const std::string filename = "test/SmallFile";
			writeSmallFile(filename);
			compressAndDecompress(filename);
		}

		void writeCharacterToFile(const std::string filename1) {
			const std::string  filename = filename1 + ".txt";
			const size_t fileSize = 10 * 1024;
			std::ofstream outFile(filename, std::ios::binary);
			if (!outFile) {
				MessageBoxW(NULL, L"Error opening file:", L"Error_Test", MB_YESNO | MB_ICONERROR);
				exit(1);
			}
			for (size_t i = 0; i < fileSize; ++i) {
				outFile.put('z');
			}
			outFile.close();
		}

		void testOneCharacterFile()
		{
			const std::string filename = "test/OneCharacterFile";
			writeCharacterToFile(filename);
			compressAndDecompress(filename);
		}

		void createFileWithZeros(const std::string filename1) {
			const std::string  filename = filename1 + ".txt";
			const size_t fileSize = 10 * 1024;
			std::ofstream outFile(filename, std::ios::binary);

			if (!outFile) {
				MessageBoxW(NULL, L"Error opening file:", L"Error_Test", MB_YESNO | MB_ICONERROR);
				exit(1);
			}
			char zero = 0;
			for (size_t i = 0; i < fileSize; ++i) {
				outFile.write(&zero, sizeof(zero));
			}
			outFile.close();

		}

		void testZeroFile()
		{
			const std::string  filename = "test/zeroFile";
			createFileWithZeros(filename);
			compressAndDecompress(filename);

		}

		void createControl_Z_File(const std::string& filename1) {
			const std::string  filename = filename1 + ".txt";
			const std::string text = "It is with great excitement that we present to you our crazy project.\nA lot of energy and effort was invested in calculating the algorithm\n";
			const size_t fileSize = 10 * 1024;
			const size_t textSize = text.size();
			const char eofChar = 0x1A; //EOF
			std::ofstream outFile(filename, std::ios::binary);
			if (!outFile) {
				MessageBoxW(NULL, L"Error opening file", L"Error_Test", MB_YESNO | MB_ICONERROR);
				exit(1);
			}
			for (size_t i = 0; i < fileSize / 2; ++i) {
				outFile.write(text.c_str(), textSize);
			}
			outFile.put(eofChar); // writë-EOF
			for (size_t i = 0; i < fileSize / 2; ++i) {
				outFile.write(text.c_str(), textSize);
			}

			outFile.close();
		}

		void testControl_Z_File()
		{
			const std::string  filename = "test/Control_Z_File";
			createControl_Z_File(filename);
			compressAndDecompress(filename);
		}

		void createSizeGBFile(const std::string& filename1) {
			const std::string  filename = filename1 + ".txt";
			const std::string text = "It is with great excitement that we present to you our crazy project.A lot of energy and effort was invested in calculating the algorithm";
			const size_t fileSize = 1024 * 1024 * 1024;
			const size_t textSize = text.size();
			std::ofstream outFile(filename, std::ios::binary);
			if (!outFile) {
				MessageBoxW(NULL, L" error open file", L"Error_Test", MB_YESNO | MB_ICONERROR);
				exit(1);
			}
			size_t repetitions = fileSize / textSize;
			for (size_t i = 0; i < repetitions; ++i) {
				outFile.write(text.c_str(), textSize);
			}
			outFile.close();
		}

		void testSizeGBFile() {
			const std::string filename = "test/SizeGBFile";
			createSizeGBFile(filename);
			compressAndDecompress(filename);
		}

	}

	void SystemTest::playTest()
	{
		if (!_mkdir("test") && errno == EEXIST)
			Logger::logError(Logger::CANNOT_CREATE_FOLDER);
		testRegularFile();
		testEmptyFile();
		testRandomFile();
		testSmallFile();
		testOneCharacterFile();
		testZeroFile();
		testControl_Z_File();
		// testSizeGBFile();
	}
}