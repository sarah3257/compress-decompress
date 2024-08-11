#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Deflate.h"
class Test
{
public:
	void testEmptyFile();
private:
	void createEmptyFile(const std::string& filename);
	void compress_decompressTest(const std::string& filename);
	bool areFilesEqual(const std::string& file1, const std::string& file2);

};

