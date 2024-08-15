//#include <windows.h>
//#include <commdlg.h>  //  GetOpenFileName
#include "resource.h"
#include "Logger.h"
#include "Test.h"
#include "Dialog.h"
#include "CompressionMetrics.h"
// main
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //CompressionMetrics n;
    //n.CompressionRatios("C:\\Users\\USER\\Downloads\\STzip.txt");
    Logger logger("log.txt");
    //Dialog
   // CompressionMetrics::play(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL,Dialog::DialogProc);

        return 0;
}
