#include "CompressionMetrics.h"
#include "CompressionDecompression.h"
#define M 100

double CompressionMetrics::HuffmanCompression(const std::string& fileName) {
	std::ifstream file(fileName, std::ifstream::ate | std::ifstream::binary);
	double originalFileSize = file.tellg();
	CompressionDecompression::compress(fileName, Huffman::compress);
	std::ifstream zipFile(fileName + "(zip)", std::ifstream::ate | std::ifstream::binary);
	double zipFileSize = file.tellg();
	return M - (zipFileSize * M / originalFileSize);
}

double CompressionMetrics::LZ77Compression(const std::string& fileName) {
	std::ifstream file(fileName, std::ifstream::ate | std::ifstream::binary);
	double originalFileSize = file.tellg();
	CompressionDecompression::compress(fileName, LZ77::compress);
	std::ifstream zipFile(fileName + "(zip)", std::ifstream::ate | std::ifstream::binary);
	double zipFileSize = file.tellg();
	return M - (zipFileSize * M / originalFileSize);
}

double CompressionMetrics::DeflateCompression(const std::string& fileName) {
	std::ifstream file(fileName, std::ifstream::ate | std::ifstream::binary);
	double originalFileSize = file.tellg();
	CompressionDecompression::compress(fileName, Deflate::compress);
	std::ifstream zipFile(fileName + "(zip)", std::ifstream::ate | std::ifstream::binary);
	double zipFileSize = file.tellg();
	return M - (zipFileSize * M / originalFileSize);
}

// the graph code need to be in the main:


// נתונים לדוגמה לשרטוט
struct Point {
	int x;
	int y;
};

std::vector<Point> dataPoints = {
	{50, 300}, {100, 250}, {150, 200}, {200, 150},
	{250, 100}, {300, 150}, {350, 200}, {400, 250},
	{450, 300}
};

std::vector<Point> dataPoints2 = {
	{50, 250}, {100, 200}, {150, 150}, {200, 100}
};
void CompressionMetrics::DrawGraph(HDC hdc) {
	// הגדרת צבעים ורוחב קווים
	HPEN hPenGrid = CreatePen(PS_DASH, 1, RGB(200, 200, 200)); // קווים אפורים לגריד
	HPEN hPenLine = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));    // קו כחול לציורי הנתונים
	HPEN hPenPoints = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));   // נקודות אדומות
	HBRUSH hBrushWhite = CreateSolidBrush(RGB(255, 255, 255));  // צבע לבן למילוי הרקע

	// שמירה על מצב ה-HDC
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPenGrid);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrushWhite);

	// ציור הרקע
	RECT rect = { 0, 0, GRAPH_WIDTH, GRAPH_HEIGHT };
	FillRect(hdc, &rect, hBrushWhite);

	// ציור גריד
	for (int i = MARGIN; i < GRAPH_WIDTH - MARGIN; i += 50) {
		MoveToEx(hdc, i, MARGIN, NULL);
		LineTo(hdc, i, GRAPH_HEIGHT - MARGIN);
	}
	for (int j = MARGIN; j < GRAPH_HEIGHT - MARGIN; j += 50) {
		MoveToEx(hdc, MARGIN, j, NULL);
		LineTo(hdc, GRAPH_WIDTH - MARGIN, j);
	}

	// ציור הקווים והנקודות
	SelectObject(hdc, hPenLine);
	MoveToEx(hdc, dataPoints[0].x, dataPoints[0].y, NULL);
	for (size_t i = 1; i < dataPoints.size(); ++i) {
		LineTo(hdc, dataPoints[i].x, dataPoints[i].y);
	}
	MoveToEx(hdc, dataPoints2[0].x, dataPoints2[0].y, NULL);
	for (size_t i = 1; i < dataPoints2.size(); ++i) {
		LineTo(hdc, dataPoints2[i].x, dataPoints2[i].y);
	}

	SelectObject(hdc, hPenPoints);
	for (const auto& point : dataPoints) {
		Ellipse(hdc, point.x - 5, point.y - 5, point.x + 5, point.y + 5);
	}
	for (const auto& point : dataPoints2) {
		Ellipse(hdc, point.x - 5, point.y - 5, point.x + 5, point.y + 5);
	}
	// ציור תוויות צירים
	SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0));
	for (int i = MARGIN; i < GRAPH_WIDTH - MARGIN; i += 50) {
		std::wstring label = std::to_wstring(i);
		TextOutW(hdc, i, GRAPH_HEIGHT - MARGIN + 10, label.c_str(), label.length());
	}
	for (int j = MARGIN; j < GRAPH_HEIGHT - MARGIN; j += 50) {
		std::wstring label = std::to_wstring(GRAPH_HEIGHT - j);
		TextOutW(hdc, MARGIN - 30, j - 10, label.c_str(), label.length());
	}

	// ניקוי משאבים
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPenGrid);
	DeleteObject(hPenLine);
	DeleteObject(hPenPoints);
	DeleteObject(hBrushWhite);
}

//// פונקציה לטיפול בהודעות חלון גרפים
LRESULT CompressionMetrics::GraphWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		DrawGraph(hdc);
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



//// פונקציה ראשית של התוכנית
int __stdcall CompressionMetrics::play(HINSTANCE hInstance, int nCmdShow) {
	const wchar_t GRAPH_CLASS_NAME[] = L"GraphWindowClass";

	WNDCLASS wc = {};
	wc.lpfnWndProc = GraphWindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = GRAPH_CLASS_NAME;

	RegisterClass(&wc);

	// יצירת חלון גרפים
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




