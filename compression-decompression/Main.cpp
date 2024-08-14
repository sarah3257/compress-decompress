//#include <windows.h>
//#include <commdlg.h>  //  GetOpenFileName
#include "resource.h"
#include "Logger.h"
#include "Test.h"
#include "Dailog.h"
#include "CompressionMetrics.h"
#include <windows.h>
#include <psapi.h>
#include <iostream>

void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        std::cout << "WorkingSetSize: " << pmc.WorkingSetSize / (1024 * 1024) << " MB" << std::endl;
        std::cout << "PeakWorkingSetSize: " << pmc.PeakWorkingSetSize / (1024 * 1024) << " MB" << std::endl;
    }
    else {
        std::cerr << "Failed to get process memory info" << std::endl;
    }
}

// main
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    printMemoryUsage();
    Logger logger("log.txt");
    //Dialog
   // CompressionMetrics::play(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL,Dailog::DialogProc);

        return 0;
}
