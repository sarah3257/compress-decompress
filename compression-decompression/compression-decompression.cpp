
#include <windows.h>
#include <commdlg.h>  //  GetOpenFileName
#include <string>
#include <vector>
#include "resource.h"
#include "Deflate.h"
#include "ErrorHandle.h"

// Declaring the functions
void runFun();
void compressFun();
void decompressFun();
void uploadFile();

//Function to convert std::string to std::wstring
std::wstring s2ws(const std::string& str)
{
    std::wstring ws(str.begin(), str.end());
    return ws;
}

//Function to convert std::wstring to std::string
std::string ws2s(const std::wstring& ws)
{
    std::string str(ws.begin(), ws.end());
    return str;
}

//Handling of Dialog messages
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1)  // button Compress
        {
            MessageBoxW(hwndDlg, L"Compress button clicked", L"Info", MB_OK);
            compressFun();
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON2)  // button Decompress
        {
            MessageBoxW(hwndDlg, L"Decompress button clicked", L"Info", MB_OK);
            decompressFun();
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON3)  // button Upload File
        {
            MessageBoxW(hwndDlg, L"Upload File button clicked", L"Info", MB_OK);
            uploadFile();
            return (INT_PTR)TRUE;
        }
        break;
    case WM_CLOSE:
        EndDialog(hwndDlg, 0);
        return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}

// maimn
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //Dialog
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);
    return 0;
}

void runFun()
{
    ErrorHandle eh("log.txt");
    std::string filePath = "inputFile.txt";
    Deflate::compress(filePath);
    Deflate::decompress("inputFile(zip).bin");
    MessageBoxW(NULL, L"פרויקט עבר בהצלחה", L"הודעה", MB_OK | MB_ICONINFORMATION);
}

void compressFun()
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
        ErrorHandle eh("log.txt");
        Deflate::compress(filePathStr);
        MessageBoxW(hwndDlg, L"The file was successfully compressed", L"Message", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBoxW(hwndDlg, L"Please select a file from the list.", L"Error", MB_OK | MB_ICONERROR);
    }
}

void decompressFun()
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
        ErrorHandle eh("log.txt");
        Deflate::decompress(filePathStr);
        MessageBoxW(hwndDlg, L"The file was successfully decompressed", L"Message", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBoxW(hwndDlg, L"Please select a file from the list.", L"Error", MB_OK | MB_ICONERROR);
    }
}

void uploadFile()
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
        MessageBoxW(NULL, L"File successfully uploaded", L"Message", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBoxW(NULL, L"Failed to upload file", L"Error", MB_OK | MB_ICONERROR);
    }
}
