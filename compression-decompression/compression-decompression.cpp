#include "Deflate.h"
#include "ErrorHandle.h"
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <iostream>


int printMemoryUsage() {
	PROCESS_MEMORY_COUNTERS memInfo;
	GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
	return memInfo.WorkingSetSize / 1024;
}

int main() {

	ErrorHandle eh("log.txt");
	std::string filePath = "STzip.txt";
	int begin = printMemoryUsage();
	auto start = std::chrono::high_resolution_clock::now();
	Deflate::compress(filePath);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsedNoHashC = end - start;
	int memory = printMemoryUsage() - begin;
	begin = printMemoryUsage();
	start = std::chrono::high_resolution_clock::now();
	Deflate::decompress("STzip(zip).bin");
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsedNoHashD = end - start;
	int memory2 = printMemoryUsage();

	std::cout << "compression memory:" << memory << " KB" << std::endl;
	std::cout << "decompression memory:" << memory2 << " KB" << std::endl;
	std::cout << "compression duration:" << elapsedNoHashC.count() << "seconds" << std::endl;
	std::cout << "decompression duration:" << elapsedNoHashD.count() << "seconds" << std::endl;
}
