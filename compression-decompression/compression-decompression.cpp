#include "Deflate.h"
#include "ErrorHandle.h"

int main() {

	ErrorHandle eh("log.txt");
	Deflate::compress("inputFile.txt");
	Deflate::decompress("inputFile(zip).bin");
}
