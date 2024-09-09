#include "CompressionMetrics.h"
#include "CompressionDecompression.h"
#include "FileStream.h"

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MAXCONSTSPEED 1
#define MINCONSTSPEED 0.5
#define MAXCONSTMEMORY 100
#define MINCONSTMEMORY 50

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

void CompressionMetrics::CompressionRatios(std::string& path) {

	// deflate
	CompressionMetrics::cpuTimeDeflate = CompressionDecompression::cpuTime;
	CompressionMetrics::memoryUsageDeflate = CompressionDecompression::memoryUsage;
	CompressionMetrics::EfficiencyPercentagesDeflate = FileStream::EfficiencyPercentages;
	// lz77
	CompressionDecompression::compress(path, LZ77::compress);
	CompressionMetrics::cpuTimeLZ77 = CompressionDecompression::cpuTime;
	CompressionMetrics::memoryUsageLZ77 = CompressionDecompression::memoryUsage;
	CompressionMetrics::EfficiencyPercentagesLZ77 = FileStream::EfficiencyPercentages;
	// huffman
	CompressionDecompression::compress(path, Huffman::compress);
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

	// draw the grid
	for (int i = MARGIN; i < GRAPH_WIDTH - MARGIN; i += 25) {
		MoveToEx(hdc, i, MARGIN, NULL);
		LineTo(hdc, i, GRAPH_HEIGHT - MARGIN);
	}
	for (int j = MARGIN; j < GRAPH_HEIGHT - MARGIN; j += 25) {
		MoveToEx(hdc, MARGIN, j, NULL);
		LineTo(hdc, GRAPH_WIDTH - MARGIN, j);
	}

	// draw the lines and points
	SelectObject(hdc, hPenLine);
	//MoveToEx(hdc, dataPoints[0].x, GRAPH_HEIGHT - MARGIN - dataPoints[0].y, NULL);
	MoveToEx(hdc, static_cast<int>(dataPoints[0].x), static_cast<int>(dataPoints[0].y), NULL);
	for (size_t i = 1; i < dataPoints.size(); ++i) {
		//LineTo(hdc, dataPoints[i].x, GRAPH_HEIGHT - MARGIN - dataPoints[i].y);
		LineTo(hdc, static_cast<int>(dataPoints[i].x), static_cast<int>(dataPoints[i].y));
	}

	SelectObject(hdc, hPenPoints);
	for (size_t i = 0; i < dataPoints.size(); ++i) {
		// Draw the red point
		Ellipse(hdc, static_cast<int>(dataPoints[i].x) - 5, static_cast<int>(dataPoints[i].y) - 5, static_cast<int>(dataPoints[i].x) + 5, static_cast<int>(dataPoints[i].y) + 5);

		// Draw the label next to the point
		SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(0, 0, 0));

		std::wstring label;
		switch (i) {
		case 0:
			label = L"LZ77";
			break;
		case 1:
			label = L"Huffman";
			break;
		case 2:
			label = L"Deflate";
			break;
		default:
			label = L"Unknown";
			break;
		}

		// Adjust label position based on the point
		int labelX = static_cast<int>(dataPoints[i].x); // Adjust as needed
		int labelY = static_cast<int>(dataPoints[i].y) + 10; // Adjust as needed

		TextOutW(hdc, labelX, labelY, label.c_str(), static_cast<int>(label.length()));
	}
	// Add the title 
	SelectObject(hdc, GetStockObject(SYSTEM_FONT));
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0));
	std::wstring title = L"Compression percentages in the following algorithms";
	TextOutW(hdc, 130, 20, title.c_str(), static_cast<int>(title.length()));

	// draw the labels for the lines
	SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0));
	//for (int i = MARGIN; i < GRAPH_WIDTH - MARGIN; i += 50) {
	//	//std::wstring label = std::to_wstring(i);
	//	std::wstring label = std::to_wstring(i / 50 * 25);
	//	TextOutW(hdc, i, GRAPH_HEIGHT - MARGIN + 10, label.c_str(), static_cast<int>(label.length()));
	//}
	for (int j = MARGIN; j < GRAPH_HEIGHT - MARGIN; j += 25) {

		std::wstring label = std::to_wstring(100 - ((j - MARGIN) / 25 * 10));
		label += L"%";
		// std::wstring label = std::to_wstring(GRAPH_HEIGHT - j);
		TextOutW(hdc, MARGIN - 30, j - 10, label.c_str(), static_cast<int>(label.length()));
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

double minConstSpeed(double minimum) {
	if (minimum < 0)
		return MINCONSTSPEED;
	return minimum + MINCONSTSPEED;
}

double minConstMemory(double minimum) {
	if (minimum < 0)
		return MINCONSTMEMORY;
	return minimum + MINCONSTMEMORY;
}

void CompressionMetrics::plotComparisonGraph() {
	FILE* gnuplotPipe = _popen("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" -persist", "w");

	// Set the terminal to be non-interactive to disable scrollbars
	fprintf(gnuplotPipe, "set terminal wxt size 1000,600 noraise noninteractive\n"); // Set the size of the window to 1600x800 pixels and make it non-interactive

	// colculate the minimum and maximum range for Speed
	double minSpeed = MIN(MIN(CompressionMetrics::cpuTimeLZ77, CompressionMetrics::cpuTimeHuffman), CompressionMetrics::cpuTimeDeflate);
	minSpeed = MAX(minSpeed, 0);
	double maxSpeed = MAX(MAX(CompressionMetrics::cpuTimeLZ77, CompressionMetrics::cpuTimeHuffman), CompressionMetrics::cpuTimeDeflate);
	maxSpeed += MAXCONSTSPEED;
	if (CompressionMetrics::cpuTimeLZ77 < CompressionMetrics::cpuTimeHuffman) {
		CompressionMetrics::cpuTimeLZ77 = minConstSpeed(CompressionMetrics::cpuTimeLZ77);
	}
	else if (CompressionMetrics::cpuTimeHuffman < CompressionMetrics::cpuTimeDeflate) {
		CompressionMetrics::cpuTimeHuffman = minConstSpeed(CompressionMetrics::cpuTimeHuffman);
	}
	else {
		CompressionMetrics::cpuTimeDeflate = minConstSpeed(CompressionMetrics::cpuTimeDeflate);
	}

	// Provide data using separate datablocks for speed and memory for three algorithms
	fprintf(gnuplotPipe, "$SpeedData << EOD\n");
	fprintf(gnuplotPipe, "LZ77 %.2f\n", CompressionMetrics::cpuTimeLZ77);
	fprintf(gnuplotPipe, "Huffman %.2f\n", CompressionMetrics::cpuTimeHuffman);
	fprintf(gnuplotPipe, "Deflate %.2f\n", CompressionMetrics::cpuTimeDeflate);
	fprintf(gnuplotPipe, "EOD\n");

	// colculate the minimum and maximum range for Memory
	double minMemory = MIN(MIN(CompressionMetrics::memoryUsageLZ77, CompressionMetrics::memoryUsageHuffman), CompressionMetrics::memoryUsageDeflate);
	minMemory = MAX(minMemory, 0);
	double maxMemory = MAX(MAX(CompressionMetrics::memoryUsageLZ77, CompressionMetrics::memoryUsageHuffman), CompressionMetrics::memoryUsageDeflate);
	maxMemory += MAXCONSTMEMORY;
	if (CompressionMetrics::memoryUsageLZ77 < CompressionMetrics::memoryUsageHuffman) {
		CompressionMetrics::memoryUsageLZ77 = minConstMemory(CompressionMetrics::memoryUsageLZ77);
	}
	else if (CompressionMetrics::memoryUsageHuffman < CompressionMetrics::memoryUsageDeflate) {
		CompressionMetrics::memoryUsageHuffman = minConstMemory(CompressionMetrics::memoryUsageHuffman);
	}
	else {
		CompressionMetrics::memoryUsageDeflate = minConstMemory(CompressionMetrics::memoryUsageDeflate);
	}

	fprintf(gnuplotPipe, "$MemoryData << EOD\n");
	fprintf(gnuplotPipe, "LZ77 %.2f\n", CompressionMetrics::memoryUsageLZ77);
	fprintf(gnuplotPipe, "Huffman %.2f\n", CompressionMetrics::memoryUsageHuffman);
	fprintf(gnuplotPipe, "Deflate %.2f\n", CompressionMetrics::memoryUsageDeflate);
	fprintf(gnuplotPipe, "EOD\n");

	// Plotting the data in separate graphs with different axis segments
	fprintf(gnuplotPipe, "set multiplot layout 2,1 title 'Comparison of Compression Algorithms'\n"); // Set up a multiplot with 2 rows and 1 column

	// Plotting Speed with specific y-axis range
	fprintf(gnuplotPipe, "set title 'Speed'\n");
	//
	//fprintf(gnuplotPipe, "set xlabel 'Compression Algorithms'\n");
	fprintf(gnuplotPipe, "set ylabel 'Seconds'\n");
	fprintf(gnuplotPipe, "set style data boxes\n");
	fprintf(gnuplotPipe, "set boxwidth 0.5\n");
	fprintf(gnuplotPipe, "set yrange [%f:%f]\n", minSpeed, maxSpeed); // Set dynamic y-axis range for Speed
	fprintf(gnuplotPipe, "plot '$SpeedData' using 2:xtic(1) with boxes title 'Speed' lc rgb 'blue'\n");

	// Plotting Memory with specific y-axis range
	fprintf(gnuplotPipe, "set title 'Memory'\n");
	//
	//fprintf(gnuplotPipe, "set xlabel 'Compression Algorithms'\n");
	fprintf(gnuplotPipe, "set ylabel 'KB'\n");
	fprintf(gnuplotPipe, "set style data boxes\n");
	fprintf(gnuplotPipe, "set boxwidth 0.5\n");
	fprintf(gnuplotPipe, "set yrange [%f:%f]\n", minMemory, maxMemory); // Set dynamic y-axis range for Memory
	fprintf(gnuplotPipe, "plot '$MemoryData' using 2:xtic(1) with boxes title 'Memory' lc rgb 'green'\n");

	fprintf(gnuplotPipe, "unset multiplot\n"); // End the multiplot

	fflush(gnuplotPipe);

	// Close the gnuplot pipe
	_pclose(gnuplotPipe);
}



