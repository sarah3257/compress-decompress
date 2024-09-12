//#include <windows.h>
//#include <commdlg.h>  //  GetOpenFileName
#include "resource.h"
#include "Logger.h"
#include "SystemTest.h"
#include "Dialog.h"
#include "CompressionMetrics.h"


 //main
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	Logger logger("log.txt");

	// Dialog
     DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL,Dialog:: DialogProc);



	return 0;
}



