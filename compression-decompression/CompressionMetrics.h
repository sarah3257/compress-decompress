#pragma once
#include "LZ77.h"
#include "Huffman.h"
#include "resource.h"
#include "Logger.h"
#include "Test.h"
#include "Dailog.h"
#include <windows.h>
#include <vector>
#include <string>

#define GRAPH_WIDTH 600
#define GRAPH_HEIGHT 400
#define MARGIN 50
class CompressionMetrics
{
public:

    double HuffmanCompression(const std::string& fileName);
    double LZ77Compression(const std::string& fileName);
    double DeflateCompression(const std::string& fileName);
   static void DrawGraph(HDC hdc);
   static LRESULT CALLBACK GraphWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

   static int WINAPI play(HINSTANCE hInstance, int nCmdShow);
};

