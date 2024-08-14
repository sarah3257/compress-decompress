#pragma once
#include <windows.h>
#include <commdlg.h>  //  GetOpenFileName
#include <string>
#include <vector>
#include "resource.h"
#include "Deflate.h"
#include "Logger.h"
#include "Test.h"

class Dailog
{




	static void compressFun();

	static void decompressFun();

	static void uploadFile();

	static void   playGraph();

	static std::wstring s2ws(const std::string& str);

public:

	static std::string ws2s(const std::wstring& ws);
	//Handling of Dialog messages
	static INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	





};

