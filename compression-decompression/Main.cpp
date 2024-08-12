
//#include <windows.h>
//#include <commdlg.h>  //  GetOpenFileName
#include "resource.h"
#include "Logger.h"
#include "Test.h"
#include "Dailog.h"

// main
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Logger logger("log.txt");
    //Dialog
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL,Dailog::DialogProc);
   
        return 0;
}
