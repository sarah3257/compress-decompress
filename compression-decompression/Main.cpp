//#include <windows.h>
//#include <commdlg.h>  //  GetOpenFileName
#include "resource.h"
#include "Logger.h"
#include "Test.h"
#include "Dailog.h"
#include "CompressionMetrics.h"

// main
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Logger logger("log.txt");
    //Dialog
   // CompressionMetrics::play(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL,Dailog::DialogProc);

        return 0;
}