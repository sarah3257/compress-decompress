#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
//
#include <string>
#include <sstream>
#include "Deflate.h"

int main() {

	std::string filePath = "inputFile.txt";
	Deflate::compress(filePath);
	Deflate::decompress("inputFile(zip).bin");

	return 0;
}