#pragma once
#include <windows.h>
#include <commdlg.h> 
#include <string>
#include <vector>
#include "resource.h"
#include "Deflate.h"
#include "Logger.h"
#include "SystemTest.h"

class Dialog
{


	static void compressFun();

	static void decompressFun();

	static void uploadFile();

	static void uploadFolder();

	static void ShowGraphMetrics();

	static std::wstring s2ws(const std::string& str);

	static void UpdateProgressBar(HWND hwndPB);

	static HWND ShowProgressBar(HWND hwndDlg);

public:

	static std::string ws2s(const std::wstring& ws);
	//Handling of Dialog messages
	static INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void plotComparisonGraph(double lz77_memory, double lz77_speed, double huffman_memory, double huffman_speed, double deflate_memory, double deflate_speed);

};

