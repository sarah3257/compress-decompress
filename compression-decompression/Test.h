#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Deflate.h"
class Test
{
public:
	static void playTest();
private:
	static void createRegularFile(const std::string& filename);
	static void testEmptyFile();
	static void createEmptyFile(const std::string& filename);
	static void writeRandomValuesToTextFile(const std::string& filename1);
	static void testRandomFile();
	static void testSmallFile();
	static bool areFilesEqual(const std::string& file1, const std::string& file2);
	static void writeSmallFile(const std::string filename1);
	static void writeCharacterToFile(const std::string filename1);
	static void testOneCharacterFile();
	static void createFileWithZeros(const std::string filename1);
	static void testZeroFile();
	static void testRegularFile();
	static void testControl_Z_File();
	static void createControl_Z_File(const std::string& filename1);
	static void testSizeGBFile();
	static void createSizeGBFile(const std::string& filename1);
	static void compressAndDecompress(const std::string& filename);

};

