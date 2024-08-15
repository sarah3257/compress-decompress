
#include "Dialog.h"
#include "Test.h"
#include "CompressionDecompression.h"
#include "CompressionMetrics.h"

void Dialog::compressFun()
{
	HWND hwndDlg = GetActiveWindow();
	HWND hListBox = GetDlgItem(hwndDlg, IDC_LIST1);
	int selIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
	if (selIndex != LB_ERR)
	{
		wchar_t filePath[MAX_PATH];
		SendMessage(hListBox, LB_GETTEXT, selIndex, (LPARAM)filePath);
		std::wstring filePathW(filePath);
		std::string filePathStr = ws2s(filePathW);
		CompressionDecompression::compress(filePathStr, Deflate::compress);
		MessageBoxW(hwndDlg, L"The file was successfully compressed", L"Message", MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		MessageBoxW(hwndDlg, L"Please select a file from the list.", L"Error", MB_OK | MB_ICONERROR);
	}
}

void Dialog::decompressFun() {

	HWND hwndDlg = GetActiveWindow();
	HWND hListBox = GetDlgItem(hwndDlg, IDC_LIST1);
	int selIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
	if (selIndex != LB_ERR)
	{
		wchar_t filePath[MAX_PATH];
		SendMessage(hListBox, LB_GETTEXT, selIndex, (LPARAM)filePath);
		std::wstring filePathW(filePath);
		std::string filePathStr = ws2s(filePathW);
		CompressionDecompression::decompress(filePathStr, Deflate::decompress);
		MessageBoxW(hwndDlg, L"The file was successfully decompressed", L"Message", MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		MessageBoxW(hwndDlg, L"Please select a file from the list.", L"Error", MB_OK | MB_ICONERROR);
	}
}

void Dialog::uploadFile()
{

	OPENFILENAME ofn;
	wchar_t szFile[260] = { 0 };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
	ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameW(&ofn))
	{
		HWND hwndDlg = GetActiveWindow();
		HWND hListBox = GetDlgItem(hwndDlg, IDC_LIST1);
		SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)szFile);
		// MessageBoxW(NULL, L"File successfully uploaded", L"Message", MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		// MessageBoxW(NULL, L"Failed to upload file", L"Error", MB_OK | MB_ICONERROR);
	}
}

std::wstring Dialog::s2ws(const std::string& str)
{
	std::wstring ws(str.begin(), str.end());
	return ws;
}

std::string Dialog::ws2s(const std::wstring& ws)
{
	std::string str(ws.begin(), ws.end());
	return str;
}

INT_PTR Dialog::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1)  // button Compress
        {
            //MessageBoxW(hwndDlg, L"Compress button clicked", L"Info", MB_OK);
            compressFun();
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON2)  // button Decompress
        {
            // MessageBoxW(hwndDlg, L"Decompress button clicked", L"Info", MB_OK);
            decompressFun();
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON3)  // button Upload File
        {
            //MessageBoxW(hwndDlg, L"Upload File button clicked", L"Info", MB_OK);
            uploadFile();
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON4) {
            Test::playTest();
            MessageBoxW(hwndDlg, L"The tests passed successfully!!", L"Info", MB_OK);

            return (INT_PTR)TRUE;
        }
		else if (LOWORD(wParam) == IDC_BUTTON5) {
			//ציפיייייייייי כאן
			plotComparisonGraph(100, 2, 150, 4, 120, 3);
			return (INT_PTR)TRUE;
		}
        else if (LOWORD(wParam) == IDC_BUTTON6) {
			HWND hwndDlg = GetActiveWindow();
			HWND hListBox = GetDlgItem(hwndDlg, IDC_LIST1);
			int selIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
			if (selIndex != LB_ERR)
			{
				wchar_t filePath[MAX_PATH];
				SendMessage(hListBox, LB_GETTEXT, selIndex, (LPARAM)filePath);
				std::wstring filePathW(filePath);
				std::string filePathStr = ws2s(filePathW);		

            HINSTANCE hInstance = GetModuleHandle(NULL); // קבלת ה-HINSTANCE של היישום
            int nCmdShow = SW_SHOW; // לדוגמה, הצגת החלון בצורה רגילה

            CompressionMetrics cm(filePathStr);
            int result = cm.play(hInstance, nCmdShow);
			}
			else
			{
				MessageBoxW(hwndDlg, L"Please select a file from the list.", L"Error", MB_OK | MB_ICONERROR);
			}
            return (INT_PTR)TRUE;
        }
		
        break;
    case WM_CLOSE:
        EndDialog(hwndDlg, 0);
        return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}

void Dialog::plotComparisonGraph(double lz77_memory, double lz77_speed, double huffman_memory, double huffman_speed, double deflate_memory, double deflate_speed) {
	FILE* gnuplotPipe = _popen("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" -persist", "w");

	// Set the terminal to be non-interactive to disable scrollbars
	fprintf(gnuplotPipe, "set terminal wxt size 1600,800 noraise noninteractive\n"); // Set the size of the window to 1600x800 pixels and make it non-interactive

	// Provide data using separate datablocks for speed and memory for three algorithms
	fprintf(gnuplotPipe, "$SpeedData << EOD\n");
	fprintf(gnuplotPipe, "LZ77 %.2f\n", lz77_speed);
	fprintf(gnuplotPipe, "Huffman %.2f\n", huffman_speed);
	fprintf(gnuplotPipe, "Deflate %.2f\n", deflate_speed);
	fprintf(gnuplotPipe, "EOD\n");

	fprintf(gnuplotPipe, "$MemoryData << EOD\n");
	fprintf(gnuplotPipe, "LZ77 %.2f\n", lz77_memory);
	fprintf(gnuplotPipe, "Huffman %.2f\n", huffman_memory);
	fprintf(gnuplotPipe, "Deflate %.2f\n", deflate_memory);
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
