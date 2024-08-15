//#include <windows.h>
//#include <commdlg.h>  //  GetOpenFileName
#include "resource.h"
#include "Logger.h"
#include "Test.h"
#include "Dialog.h"
#include "CompressionMetrics.h"
#include <windows.h>
#include <psapi.h>
#include <iostream>

void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        std::cout << "Memory Usage: " << pmc.WorkingSetSize / (1024.0 * 1024.0) << " MB" << std::endl;
    }
    else {
        std::cerr << "Failed to get process memory info" << std::endl;
    }
}

int main() {
    // כאן ניתן להפעיל את התוכנית שאתה רוצה למדוד את השימוש בזיכרון שלה
    // לדוגמה:
    
    return 0;
}

// main
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    printMemoryUsage();

    Logger logger("log.txt");
    //Dialog
   // CompressionMetrics::play(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL,Dialog::DialogProc);

        return 0;
}
