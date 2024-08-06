#include "Deflate.h"

int main() {

	// an example to compress a text file
	Deflate::compress("inputFile.txt");
	Deflate::decompress("inputFile(zip).bin");

	return 0;

}