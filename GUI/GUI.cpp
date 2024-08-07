#include <windows.h>
#include <shlwapi.h> // עבור PathFindFileName
#include "Deflate.h"
#pragma comment(lib, "shlwapi.lib") // קישור לספריית Shlwapi

#define IDC_COMBO_MODE          1001
#define IDC_BUTTON_RUN          1002
#define IDC_BUTTON_CANCEL       1003
#define IDC_BUTTON_BROWSE       1004
#define IDC_EDIT_FILE_PATH      1005

HINSTANCE hInst;
HWND hComboMode;
HWND hEditFilePath;
HWND hButtonRun;

void UpdateRunButtonState() {
    WCHAR filePath[256];
    GetWindowText(hEditFilePath, filePath, sizeof(filePath) / sizeof(filePath[0]));
    BOOL enableRunButton = (lstrlen(filePath) > 0);
    EnableWindow(hButtonRun, enableRunButton);
}

void BrowseFile(HWND hwnd) {
    OPENFILENAME ofn;
    WCHAR szFile[MAX_PATH] = L"";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"Text Files\0*.TXT\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        SetWindowText(hEditFilePath, szFile);
        UpdateRunButtonState();
    }
}

void RunCompressionOrDecompression() {
    WCHAR szMode[256];
    WCHAR szFile[256];
    GetWindowText(hComboMode, szMode, sizeof(szMode) / sizeof(szMode[0]));
    GetWindowText(hEditFilePath, szFile, sizeof(szFile) / sizeof(szFile[0]));

    // חיתוך שם הקובץ ממלא הנתיב
    WCHAR szFileName[MAX_PATH];
    PathStripPath(szFile); // שימוש בפונקציה הנכונה לחיתוך שם הקובץ
    wcscpy_s(szFileName, szFile);

    WCHAR message[512];
    wsprintf(message, L"Action: %s\nFile: %s", szMode, szFileName);

    if (wcscmp(szMode, L"Compress") == 0) {
        MessageBox(NULL, message, L"Compression", MB_OK);
        // הוסיפי כאן את הקוד לדחיסה
        Deflate::compress("inputFile.txt");
    }
    else if (wcscmp(szMode, L"Decompress") == 0) {
        MessageBox(NULL, message, L"Decompression", MB_OK);
        // הוסיפי כאן את הקוד לפריסה
        Deflate::decompress("inputFile(zip).bin");
    }
    else {
        MessageBox(NULL, L"Please select an option.", L"Error", MB_OK | MB_ICONERROR);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        CreateWindow(L"STATIC", L"Select a file:", WS_VISIBLE | WS_CHILD, 10, 10, 120, 20, hwnd, NULL, hInst, NULL);
        hEditFilePath = CreateWindow(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 140, 10, 200, 20, hwnd, (HMENU)IDC_EDIT_FILE_PATH, hInst, NULL);
        CreateWindow(L"BUTTON", L"Browse", WS_CHILD | WS_VISIBLE, 350, 10, 80, 20, hwnd, (HMENU)IDC_BUTTON_BROWSE, hInst, NULL);

        hComboMode = CreateWindow(L"COMBOBOX", L"", CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE, 10, 40, 150, 100, hwnd, (HMENU)IDC_COMBO_MODE, hInst, NULL);
        SendMessage(hComboMode, CB_ADDSTRING, 0, (LPARAM)L"Compress");
        SendMessage(hComboMode, CB_ADDSTRING, 0, (LPARAM)L"Decompress");
        SendMessage(hComboMode, CB_SETCURSEL, 0, 0); // ברירת מחדל היא Compress

        hButtonRun = CreateWindow(L"BUTTON", L"Run & Download", WS_CHILD | WS_VISIBLE, 10, 80, 120, 30, hwnd, (HMENU)IDC_BUTTON_RUN, hInst, NULL);
        CreateWindow(L"BUTTON", L"Cancel", WS_CHILD | WS_VISIBLE, 140, 80, 120, 30, hwnd, (HMENU)IDC_BUTTON_CANCEL, hInst, NULL);

        UpdateRunButtonState(); // לעדכן את מצב הכפתור כשנפתח החלון
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BUTTON_BROWSE:
            BrowseFile(hwnd);
            break;
        case IDC_BUTTON_RUN:
            RunCompressionOrDecompression();
            break;
        case IDC_BUTTON_CANCEL:
            PostQuitMessage(0);
            break;
        case IDC_EDIT_FILE_PATH:
            if (HIWORD(wParam) == EN_CHANGE) {
                UpdateRunButtonState();
            }
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HWND hwnd;
    MSG msg;
    WNDCLASS wc = { };

    hInst = hInstance;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"Sample Window Class";
    RegisterClass(&wc);

    hwnd = CreateWindowEx(0, L"Sample Window Class", L"File Compressor/Decompressor", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 450, 200, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
