#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
//
#include <string>
#include <sstream>
#include "Deflate.h"

int main() {

    // an example to compress a binary file
	
	//std::ifstream inputFile("inputFile.txt");
	//if (!inputFile) {
	//	std::cerr << "the text file dont open" << std::endl;
	//	return 1;
	//}

	//std::ostringstream oss;
	//oss << inputFile.rdbuf();
	//std::string data = oss.str();
	//inputFile.close();

	//// write the content to the binary file
	//std::ofstream outputFile("example.bin", std::ios::binary);
	//if (!outputFile) {
	//	std::cerr << "the binary file dont open" << std::endl;
	//	return 1;
	//}

	//size_t length = data.size();
	//outputFile.write(data.c_str(), length);
	//outputFile.close();

	//std::string filePath = "example.bin";
	//Deflate::compress(filePath);
	//Deflate::decompress("example(zip).bin");

	// an example to compress a text file
	std::string filePath = "inputFile.txt";
	Deflate::compress(filePath);
	Deflate::decompress("inputFile(zip).bin");

	return 0;

}