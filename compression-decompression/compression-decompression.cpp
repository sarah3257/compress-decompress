#include <iostream>
#include <string>
#include "Deflate.h"

int main() {
	ErrorHandle eh("log.txt");
	std::string filePath = "inputFile.txt";
	Deflate::compress(filePath);
	Deflate::decompress("inputFile(zip).bin");
}
