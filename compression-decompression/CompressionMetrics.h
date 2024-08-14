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
    static const std::string password;

    double compressionRatio; // ratio
    double compressionSpeed; // speed
    double originalSize;     // original size
    double compressedSize;   // compress size

public:

    CompressionMetrics()
    {

    }

    double HuffmanCompression(const std::string& fileName);
    void LZ77Compression(const std::string& fileName);


    void setMetrics(double originalSize, double compressedSize, double compressionSpeed) {
        this->originalSize = originalSize;
        this->compressedSize = compressedSize;
        this->compressionSpeed = compressionSpeed;
        if (originalSize > 0) {
            this->compressionRatio = (originalSize - compressedSize) / originalSize * 100.0;
        }
        else {
            this->compressionRatio = 0.0;
        }
    }

    double getCompressionRatio() const {
        return compressionRatio;
    }

    double getCompressionSpeed() const {
        return compressionSpeed;
    }

    double getOriginalSize() const {
        return originalSize;
    }

    double getCompressedSize() const {
        return compressedSize;
    }

   static void DrawGraph(HDC hdc);
   static LRESULT CALLBACK GraphWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

   static int WINAPI play(HINSTANCE hInstance, int nCmdShow);
};

