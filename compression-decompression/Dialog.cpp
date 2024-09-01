
#include "Dialog.h"
#include "Test.h"
#include "CompressionDecompression.h"
#include "CompressionMetrics.h"
#include <windows.h>  
#include <shlobj.h>  

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
		else if (LOWORD(wParam) == IDC_BUTTONFOLDER) {
			//MessageBoxW(hwndDlg, L"Upload folder button clicked", L"Info", MB_OK);
			uploadFolder();
			return (INT_PTR)TRUE;


		}
        else if (LOWORD(wParam) == IDC_BUTTON4) {
            Test::playTest();
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
