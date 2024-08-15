#include "CompressionMetrics.h"
#include "CompressionDecompression.h"
#include "FileStream.h"

#define M 100
std::string CompressionMetrics::fileName;

double CompressionMetrics::EfficiencyPercentages() {
	return M - (FileStream::destinationFileSize * M / FileStream::originalFileSize);
}

double CompressionMetrics::HuffmanCompression() {
	CompressionDecompression::compress(fileName, Huffman::compress);
	return EfficiencyPercentages();
}

double CompressionMetrics::LZ77Compression() {
	CompressionDecompression::compress(fileName, LZ77::compress);
	return EfficiencyPercentages();
}

double CompressionMetrics::DeflateCompression() {
	CompressionDecompression::compress(fileName, Deflate::compress);
	return EfficiencyPercentages();
}



//point to DrawGraph
struct Point {
    int x;
    double y;
};




void CompressionMetrics::DrawGraph(HDC hdc, double percentLZ77,double percentHuffman, double percentDeflate) {

    std::vector<Point> dataPoints = {
    {50, percentLZ77}, {250, percentHuffman}, {450, percentDeflate}
    };
    // ����� ����� ����� �����
    HPEN hPenGrid = CreatePen(PS_DASH, 1, RGB(200, 200, 200)); // ����� ������ �����
    HPEN hPenLine = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));    // �� ���� ������ �������
    HPEN hPenPoints = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));   // ������ ������
    HBRUSH hBrushWhite = CreateSolidBrush(RGB(255, 255, 255));  // ��� ��� ������ ����

	// ����� �� ��� �-HDC
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPenGrid);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrushWhite);

	// ���� ����
	RECT rect = { 0, 0, GRAPH_WIDTH, GRAPH_HEIGHT };
	FillRect(hdc, &rect, hBrushWhite);

    // ���� ����
    for (int i = MARGIN; i < GRAPH_WIDTH - MARGIN; i += 25) {
        MoveToEx(hdc, i, MARGIN, NULL);
        LineTo(hdc, i, GRAPH_HEIGHT - MARGIN);
    }
    for (int j = MARGIN; j < GRAPH_HEIGHT - MARGIN; j += 25) {
        MoveToEx(hdc, MARGIN, j, NULL);
        LineTo(hdc, GRAPH_WIDTH - MARGIN, j);
    }

    // ���� ������ ��������
    SelectObject(hdc, hPenLine);
    //MoveToEx(hdc, dataPoints[0].x, GRAPH_HEIGHT - MARGIN - dataPoints[0].y, NULL);
    MoveToEx(hdc, dataPoints[0].x, dataPoints[0].y, NULL);
    for (size_t i = 1; i < dataPoints.size(); ++i) {
        //LineTo(hdc, dataPoints[i].x, GRAPH_HEIGHT - MARGIN - dataPoints[i].y);
       LineTo(hdc, dataPoints[i].x, dataPoints[i].y);
    }
 

    SelectObject(hdc, hPenPoints);
    for (const auto& point : dataPoints) {
        Ellipse(hdc, point.x - 5, point.y - 5, point.x + 5, point.y + 5);
    }
 
    // ���� ������ �����
    SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 0, 0));
    for (int i = MARGIN; i < GRAPH_WIDTH - MARGIN; i += 50) {
        //std::wstring label = std::to_wstring(i);
        std::wstring label = std::to_wstring(i / 50 * 25);
        TextOutW(hdc, i, GRAPH_HEIGHT - MARGIN + 10, label.c_str(), label.length());
    }
    for (int j = MARGIN; j < GRAPH_HEIGHT - MARGIN; j += 25) {

        std::wstring label = std::to_wstring(100 - ((j - MARGIN) / 25 * 10));
       // std::wstring label = std::to_wstring(GRAPH_HEIGHT - j);
        TextOutW(hdc, MARGIN - 30, j - 10, label.c_str(), label.length());
    }

	// ����� ������
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPenGrid);
	DeleteObject(hPenLine);
	DeleteObject(hPenPoints);
	DeleteObject(hBrushWhite);
}

//// ������� ������ ������� ���� �����
LRESULT CompressionMetrics::GraphWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    double percentLZ77 = 300-LZ77Compression()*2, percentHuffman = 300-HuffmanCompression()*2, percentDeflate = 300- DeflateCompression()*2;
    switch (uMsg) {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
    
        DrawGraph(hdc, percentLZ77, percentHuffman, percentDeflate);
        EndPaint(hwnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}



int __stdcall CompressionMetrics::play(HINSTANCE hInstance, int nCmdShow) {
	const wchar_t GRAPH_CLASS_NAME[] = L"GraphWindowClass";

	WNDCLASS wc = {};
	wc.lpfnWndProc = GraphWindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = GRAPH_CLASS_NAME;

	RegisterClass(&wc);

	// ����� ���� �����
	HWND hwndGraph = CreateWindowEx(0, GRAPH_CLASS_NAME, L"Graph Window", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, GRAPH_WIDTH, GRAPH_HEIGHT,
		NULL, NULL, hInstance, NULL);

	if (hwndGraph == NULL) {
		return 0;
	}

	ShowWindow(hwndGraph, nCmdShow);
	UpdateWindow(hwndGraph);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}




