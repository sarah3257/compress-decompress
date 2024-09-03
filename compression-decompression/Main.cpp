//#include <windows.h>
//#include <commdlg.h>  //  GetOpenFileName
#include "resource.h"
#include "Logger.h"
#include "Test.h"
#include "Dialog.h"
#include "CompressionMetrics.h"

// main
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ int nCmdShow)
{
	Logger logger("log.txt");
	//Dialog
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Dialog::DialogProc);

	return 0;
}
