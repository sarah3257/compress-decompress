
#include "Dialog.h"
//#include "Test.h"
#include "CompressionDecompression.h"
#include "CompressionMetrics.h"
#include <windows.h>  
#include <shlobj.h>  
#include <commctrl.h> 

// ParseALargeFile - uses a progress bar to indicate the progress of a parsing operation.
//
// Returns TRUE if successful, or FALSE otherwise.
//
// hwndParent - parent window of the progress bar.
//
// lpszFileName - name of the file to parse. 
// 
// Global variable 
//     g_hinst - instance handle.
//

extern HINSTANCE g_hinst;
HINSTANCE g_hinst = nullptr;

BOOL ParseALargeFile(HWND hwndParent, LPTSTR lpszFileName)
{
	RECT rcClient;  // Client area of parent window.
	int cyVScroll;  // Height of scroll bar arrow.
	HWND hwndPB;    // Handle of progress bar.
	HANDLE hFile;   // Handle of file.
	DWORD cb;       // Size of file and count of bytes read.
	LPCH pch;       // Address of data read from file.
	LPCH pchTmp;    // Temporary pointer.

	// Ensure that the common control DLL is loaded, and create a progress bar 
	// along the bottom of the client area of the parent window. 
	//
	// Base the height of the progress bar on the height of a scroll bar arrow.

	InitCommonControls();

	GetClientRect(hwndParent, &rcClient);

	cyVScroll = GetSystemMetrics(SM_CYVSCROLL);

	hwndPB = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR)NULL,
		WS_CHILD | WS_VISIBLE, rcClient.left,
		rcClient.bottom - cyVScroll,
		rcClient.right, cyVScroll,
		hwndParent, (HMENU)0, g_hinst, NULL);

	if (!hwndPB) {
		MessageBox(hwndParent, L"Failed to create progress bar", L"Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// Open the file for reading, and retrieve the size of the file. 

	hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if (hFile == (HANDLE)INVALID_HANDLE_VALUE)
		return FALSE;

	cb = GetFileSize(hFile, (LPDWORD)NULL);

	// Set the range and increment of the progress bar. 

	SendMessage(hwndPB, PBM_SETRANGE, 0, MAKELPARAM(0, cb / 2048));

	SendMessage(hwndPB, PBM_SETSTEP, (WPARAM)1, 0);

	// Parse the file. 
	pch = (LPCH)LocalAlloc(LPTR, sizeof(char) * 2048);

	pchTmp = pch;

	do {
		ReadFile(hFile, pchTmp, sizeof(char) * 2048, &cb, (LPOVERLAPPED)NULL);

		// TODO: Write an error handler to check that all the
		// requested data was read.
		//
		// Include here any code that parses the
		// file. 
		//  
		//  
		//  
		// Advance the current position of the progress bar by the increment.

		SendMessage(hwndPB, PBM_STEPIT, 0, 0);

	} while (cb);

	CloseHandle((HANDLE)hFile);

	DestroyWindow(hwndPB);

	return TRUE;
}
//HINSTANCE g_hinst = NULL;

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

		// פתיחת סרגל ההתקדמות
		BOOL success = ParseALargeFile(hwndDlg, filePath);


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

		// פתיחת סרגל ההתקדמות
		BOOL success = ParseALargeFile(hwndDlg, filePath);

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

	OPENFILENAME ofn;//Setting parameters for the dialog to open a file
	wchar_t szFile[260] = { 0 };//Array for saving routing

	ZeroMemory(&ofn, sizeof(ofn));//Resets the struct ofn
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
void Dialog::uploadFolder()
{
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(bi));

	bi.lpszTitle = L"Select Folder to Upload";
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.hwndOwner = GetActiveWindow();

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != NULL)
	{
		wchar_t szFolder[MAX_PATH];
		if (SHGetPathFromIDList(pidl, szFolder))
		{
			// הוסף את נתיב התיקיה לרשימה
			HWND hwndDlg = GetActiveWindow();
			HWND hListBox = GetDlgItem(hwndDlg, IDC_LIST1);
			SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)szFolder);

			// הצגת הודעה לתיקיה שהועלתה בהצלחה
			// MessageBoxW(NULL, L"Folder successfully uploaded", L"Message", MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			// הצגת הודעת שגיאה אם לא הצליח להעלות תיקיה
			// MessageBoxW(NULL, L"Failed to upload folder", L"Error", MB_OK | MB_ICONERROR);
		}

		// שחרור הזיכרון שהוקצה עבור ה-pidl
		IMalloc* imalloc = NULL;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}
	}
}
//
//BOOL ParseALargeFile(HWND hwndParent, LPTSTR lpszFileName)
//{
//	RECT rcClient;
//	int cyVScroll;
//	HWND hwndPB;
//	HANDLE hFile;
//	DWORD cb;
//	LPCH pch;
//	LPCH pchTmp;
//
//	InitCommonControls();
//
//	GetClientRect(hwndParent, &rcClient);
//
//	cyVScroll = GetSystemMetrics(SM_CYVSCROLL);
//
//	hwndPB = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR)NULL,
//		WS_CHILD | WS_VISIBLE, rcClient.left,
//		rcClient.bottom - cyVScroll,
//		rcClient.right, cyVScroll,
//		hwndParent, (HMENU)0, g_hinst, NULL);
//
//	hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ,
//		(LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING,
//		FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
//
//	if (hFile == (HANDLE)INVALID_HANDLE_VALUE)
//		return FALSE;
//
//	cb = GetFileSize(hFile, (LPDWORD)NULL);
//
//	SendMessage(hwndPB, PBM_SETRANGE, 0, MAKELPARAM(0, cb / 2048));
//
//	SendMessage(hwndPB, PBM_SETSTEP, (WPARAM)1, 0);
//
//	pch = (LPCH)LocalAlloc(LPTR, sizeof(char) * 2048);
//
//	pchTmp = pch;
//
//	do {
//		ReadFile(hFile, pchTmp, sizeof(char) * 2048, &cb, (LPOVERLAPPED)NULL);
//
//		SendMessage(hwndPB, PBM_STEPIT, 0, 0);
//
//	} while (cb);
//
//	CloseHandle((HANDLE)hFile);
//
//	DestroyWindow(hwndPB);
//
//	return TRUE;
//}

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
			wchar_t filePath[MAX_PATH];
			BOOL success = ParseALargeFile(hwndDlg, filePath);

			decompressFun();
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BUTTON3)  // button Upload File
		{
			//MessageBoxW(hwndDlg, L"Upload File button clicked", L"Info", MB_OK);
			uploadFile();
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BUTTONFOLDER) {
			//MessageBoxW(hwndDlg, L"Upload folder button clicked", L"Info", MB_OK);
			uploadFolder();
			return (INT_PTR)TRUE;


		}
		else if (LOWORD(wParam) == IDC_BUTTON4) {
		//	Test::playTest();
			MessageBoxW(hwndDlg, L"The tests passed successfully!!", L"Info", MB_OK);

			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BUTTON5) {
			CompressionMetrics::plotComparisonGraph();
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

