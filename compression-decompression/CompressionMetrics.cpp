#include "CompressionMetrics.h"
#include "CompressionDecompression.h"
#include "FileStream.h"

double CompressionMetrics::cpuTimeDeflate = 0;
double CompressionMetrics::memoryUsageDeflate = 0;
double CompressionMetrics::EfficiencyPercentagesDeflate = 0;
double CompressionMetrics::cpuTimeLZ77 = 0;
double CompressionMetrics::memoryUsageLZ77 = 0;
double CompressionMetrics::EfficiencyPercentagesLZ77 = 0;
double CompressionMetrics::cpuTimeHuffman = 0;
double CompressionMetrics::memoryUsageHuffman = 0;
double CompressionMetrics::EfficiencyPercentagesHuffman = 0;

std::string CompressionMetrics::fileName;

void CompressionMetrics::CompressionRatios(std::string& fileName) {

	// deflate
	CompressionMetrics::cpuTimeDeflate = CompressionDecompression::cpuTime;
	CompressionMetrics::memoryUsageDeflate = CompressionDecompression::memoryUsage;
	CompressionMetrics::EfficiencyPercentagesDeflate = FileStream::EfficiencyPercentages;
	// lz77
	CompressionDecompression::compress(fileName, LZ77::compress);
	CompressionMetrics::cpuTimeLZ77 = CompressionDecompression::cpuTime;
	CompressionMetrics::memoryUsageLZ77 = CompressionDecompression::memoryUsage;
	CompressionMetrics::EfficiencyPercentagesLZ77 = FileStream::EfficiencyPercentages;
	// huffman
	CompressionDecompression::compress(fileName, Huffman::compress);
	CompressionMetrics::cpuTimeHuffman = CompressionDecompression::cpuTime;
	CompressionMetrics::memoryUsageHuffman = CompressionDecompression::memoryUsage;
	CompressionMetrics::EfficiencyPercentagesHuffman = FileStream::EfficiencyPercentages;
}

//point to DrawGraph
struct Point {
	int x;
	double y;
};




void CompressionMetrics::DrawGraph(HDC hdc, double percentLZ77, double percentHuffman, double percentDeflate) {

	std::vector<Point> dataPoints = {
	{50, percentLZ77}, {250, percentHuffman}, {450, percentDeflate}
	};
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
	for (int i = MARGIN; i < GRAPH_WIDTH - MARGIN; i += 25) {
		MoveToEx(hdc, i, MARGIN, NULL);
		LineTo(hdc, i, GRAPH_HEIGHT - MARGIN);
	}
	for (int j = MARGIN; j < GRAPH_HEIGHT - MARGIN; j += 25) {
		MoveToEx(hdc, MARGIN, j, NULL);
		LineTo(hdc, GRAPH_WIDTH - MARGIN, j);
	}

	// ציור הקווים והנקודות
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

	// ציור תוויות צירים
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

	double percentLZ77 = 300 - CompressionMetrics::EfficiencyPercentagesLZ77 * 2, percentHuffman = 300 - CompressionMetrics::EfficiencyPercentagesHuffman * 2, percentDeflate = 300 - CompressionMetrics::EfficiencyPercentagesDeflate * 2;
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


void CompressionMetrics::plotComparisonGraph() {
	FILE* gnuplotPipe = _popen("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" -persist", "w");

	// Set the terminal to be non-interactive to disable scrollbars
	fprintf(gnuplotPipe, "set terminal wxt size 1600,800 noraise noninteractive\n"); // Set the size of the window to 1600x800 pixels and make it non-interactive

	// Provide data using separate datablocks for speed and memory for three algorithms
	double maxCpuTime = min(CompressionMetrics::cpuTimeLZ77, CompressionMetrics::cpuTimeHuffman, CompressionMetrics::cpuTimeDeflate);
	fprintf(gnuplotPipe, "$SpeedData << EOD\n");
	fprintf(gnuplotPipe, "LZ77 %.2f\n", CompressionMetrics::cpuTimeLZ77 / 1000000);
	fprintf(gnuplotPipe, "Huffman %.2f\n", CompressionMetrics::cpuTimeHuffman / 1000000);
	fprintf(gnuplotPipe, "Deflate %.2f\n", CompressionMetrics::cpuTimeDeflate / 1000000);
	fprintf(gnuplotPipe, "EOD\n");

	double maxMemoryUsage = min(CompressionMetrics::memoryUsageLZ77, CompressionMetrics::memoryUsageHuffman, CompressionMetrics::memoryUsageDeflate);
	fprintf(gnuplotPipe, "$MemoryData << EOD\n");
	fprintf(gnuplotPipe, "LZ77 %.2f\n", CompressionMetrics::memoryUsageLZ77/20);
	fprintf(gnuplotPipe, "Huffman %.2f\n", CompressionMetrics::memoryUsageHuffman/20);
	fprintf(gnuplotPipe, "Deflate %.2f\n", CompressionMetrics::memoryUsageDeflate/20);
	fprintf(gnuplotPipe, "EOD\n");

	// Plotting the data in separate graphs with different axis segments
	fprintf(gnuplotPipe, "set multiplot layout 2,1 title 'Comparison of Compression Algorithms'\n"); // Set up a multiplot with 2 rows and 1 column

	// Plotting Speed with specific y-axis range
	fprintf(gnuplotPipe, "set title 'Speed'\n");
	fprintf(gnuplotPipe, "set xlabel 'Compression Algorithms'\n");
	fprintf(gnuplotPipe, "set ylabel 'Speed'\n");
	fprintf(gnuplotPipe, "set style data boxes\n");
	fprintf(gnuplotPipe, "set boxwidth 0.5\n");
	fprintf(gnuplotPipe, "set yrange [0:6]\n"); // Set specific y-axis range for Speed
	fprintf(gnuplotPipe, "plot '$SpeedData' using 2:xtic(1) with boxes title 'Speed' lc rgb 'blue'\n");

	// Plotting Memory with specific y-axis range
	fprintf(gnuplotPipe, "set title 'Memory'\n");
	fprintf(gnuplotPipe, "set xlabel 'Compression Algorithms'\n");
	fprintf(gnuplotPipe, "set ylabel 'Memory'\n");
	fprintf(gnuplotPipe, "set style data boxes\n");
	fprintf(gnuplotPipe, "set boxwidth 0.5\n");
	fprintf(gnuplotPipe, "set yrange [0:200]\n"); // Set specific y-axis range for Memory
	fprintf(gnuplotPipe, "plot '$MemoryData' using 2:xtic(1) with boxes title 'Memory' lc rgb 'green'\n");

	fprintf(gnuplotPipe, "unset multiplot\n"); // End the multiplot

	fflush(gnuplotPipe);

	// Close the gnuplot pipe
	_pclose(gnuplotPipe);
}



