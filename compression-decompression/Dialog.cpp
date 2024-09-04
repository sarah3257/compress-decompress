
#include "Dialog.h"
#include "CompressionDecompression.h"
#include "CompressionMetrics.h"
#include <windows.h>  
#include <shlobj.h>  
#include <commctrl.h> 
#include <thread>
#include <atomic>
#include "SystemTest.h"

std::atomic<bool> compressionInProgress(false);

extern HINSTANCE g_hinst = nullptr;

void compressInBackground(const std::string& filePathStr) {
	CompressionDecompression::compress(filePathStr, Deflate::compress);
	compressionInProgress = false;
}

void DecompressInBackground(const std::string& filePathStr) {
	CompressionDecompression::decompress(filePathStr, Deflate::decompress);
	compressionInProgress = false;
}

void Dialog::UpdateProgressBar(HWND hwndPB) {

	while (compressionInProgress) {
		SendMessage(hwndPB, PBM_STEPIT, 0, 0);
		Sleep(100);
	}

	// finishing decompressing and hiding the progress bar
	DestroyWindow(hwndPB);
}

HWND Dialog::ShowProgressBar(HWND hwndDlg) {

	RECT rcClient;
	int cyVScroll;
	HWND hwndPB;

	GetClientRect(hwndDlg, &rcClient);
	cyVScroll = GetSystemMetrics(SM_CYVSCROLL);

	hwndPB = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR)NULL,
		WS_CHILD | WS_VISIBLE, rcClient.left,
		rcClient.bottom - cyVScroll,
		rcClient.right, cyVScroll,
		hwndDlg, (HMENU)0, g_hinst, NULL);

	if (!hwndPB) {
		return NULL;
	}

	// Set progress bar properties
	SendMessage(hwndPB, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
	SendMessage(hwndPB, PBM_SETSTEP, (WPARAM)1, 0);
	SendMessage(hwndPB, PBM_SETPOS, 0, 0);

	return hwndPB;
}

void Dialog::compressFun()
{

	HWND hwndDlg = GetActiveWindow();
	HWND hListBox = GetDlgItem(hwndDlg, IDC_LIST1);
	int selIndex = static_cast<int>(SendMessage(hListBox, LB_GETCURSEL, 0, 0));
	if (selIndex != LB_ERR)
	{
		wchar_t filePath[MAX_PATH] = { 0 };
		SendMessage(hListBox, LB_GETTEXT, selIndex, (LPARAM)filePath);
		filePath[MAX_PATH - 1] = L'\0';

		std::wstring filePathW(filePath);
		std::string filePathStr = ws2s(filePathW);

		HWND hwndPB = ShowProgressBar(hwndDlg);

		// start the compress in another thread
		compressionInProgress = true;
		std::thread compressionThread(compressInBackground, filePathStr);
		compressionThread.detach(); // disconnecting the tread so that it doesn't take up an unnecessary memory resource
		UpdateProgressBar(hwndPB);
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
	int selIndex = static_cast<int>(SendMessage(hListBox, LB_GETCURSEL, 0, 0));
	if (selIndex != LB_ERR)
	{
		wchar_t filePath[MAX_PATH] = { 0 };
		SendMessage(hListBox, LB_GETTEXT, selIndex, (LPARAM)filePath);
		std::wstring filePathW(filePath);
		std::string filePathStr = ws2s(filePathW);

		HWND hwndPB = ShowProgressBar(hwndDlg);

		// start the decompress in another thread
		compressionInProgress = true;
		std::thread decompressionThread(DecompressInBackground, filePathStr);
		decompressionThread.detach(); // disconnecting the tread so that it doesn't take up an unnecessary memory resource
		UpdateProgressBar(hwndPB);
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

std::wstring Dialog::s2ws(const std::string& str)
{
	std::wstring ws(str.begin(), str.end());
	return ws;
}

std::string Dialog::ws2s(const std::wstring& ws) {
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &ws[0], (int)ws.size(), nullptr, 0, nullptr, nullptr);
	std::string str(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &ws[0], (int)ws.size(), &str[0], size_needed, nullptr, nullptr);
	return str;
}
INT_PTR Dialog::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM )
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
		else if (LOWORD(wParam) == IDC_BUTTON4) {//button Test
			SystemTest::playTest();
			MessageBoxW(hwndDlg, L"The tests passed successfully!!", L"Info", MB_OK);

			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL) {//button cancel

			int result = MessageBoxW(hwndDlg, L"Are you sure you want to cancel?", L"Warning", MB_OKCANCEL | MB_ICONINFORMATION);
			if (result == IDOK) {
				EndDialog(hwndDlg, 0);
			}
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BUTTON5) {
			CompressionMetrics::plotComparisonGraph();
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BUTTON6) {
			HWND localHwndDlg = GetActiveWindow();
			HWND hListBox = GetDlgItem(localHwndDlg, IDC_LIST1);
			int selIndex = static_cast<int>(SendMessage(hListBox, LB_GETCURSEL, 0, 0));
			if (selIndex != LB_ERR)
			{
				wchar_t filePath[MAX_PATH] = { 0 };
				SendMessage(hListBox, LB_GETTEXT, selIndex, (LPARAM)filePath);
				std::wstring filePathW(filePath);
				std::string filePathStr = ws2s(filePathW);

				CompressionMetrics cm(filePathStr);
			}
			else
			{
				MessageBoxW(localHwndDlg, L"Please select a file from the list.", L"Error", MB_OK | MB_ICONERROR);
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

