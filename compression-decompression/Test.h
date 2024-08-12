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
	static void testEmptyFile();
	static void createEmptyFile(const std::string& filename);
	static void writeRandomValuesToTextFile();
	static void testRandomFile();
	static void testSmallFile();
	static bool areFilesEqual(const std::string& file1, const std::string& file2);
	static void writeSmallFile();

};

