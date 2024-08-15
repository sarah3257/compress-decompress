#pragma once
#include "LZ77.h"
#include "Huffman.h"
#include "resource.h"
#include "Logger.h"
#include "Test.h"
#include "Dialog.h"
#include <windows.h>
#include <vector>
#include <string>

#define GRAPH_WIDTH 600
#define GRAPH_HEIGHT 400
#define MARGIN 50

class CompressionMetrics
{
public:


	static double cpuTimeDeflate;
	static double memoryUsageDeflate;
	static double EfficiencyPercentagesDeflate;
	static double cpuTimeLZ77;
	static double memoryUsageLZ77;
	static double EfficiencyPercentagesLZ77;
	static double cpuTimeHuffman;
	static double memoryUsageHuffman;
	static double EfficiencyPercentagesHuffman;

	void CompressionRatios(const std::string& fileName);

	static void DrawGraph(HDC hdc, double percentLZ77, double percentHuffman, double percentDeflate);


    static LRESULT CALLBACK GraphWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

   static int WINAPI play(HINSTANCE hInstance, int nCmdShow, const std::string& fileName);
};

