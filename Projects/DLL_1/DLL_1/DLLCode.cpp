#include <windows.h>																	// To work with Windows API

BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)				// BOOL WINAPI DllMain: An optional entry point into a dynamic-link library (DLL). When the system starts or terminates a process or thread, it calls the entry-point function for each loaded DLL using the first thread of the process. The system also calls the entry-point function for a DLL when it is loaded or unloaded using the LoadLibrary and FreeLibrary functions.
{
	int stopCy = 0;
	//char buff[20];
	while (stopCy < 1)
	{
		if (fdwReason == DLL_PROCESS_ATTACH)											// If the DLL has loaded into the process(DLL_PROCESS_ATTACH = 1 if loaded and LL_PROCESS_ATTACH = 0 if not)
		{
			MessageBox(0, L"Injected By Anonymous %d", L"Hello World", MB_OK);			// There is L before the text because its wide characters
			stopCy += 1;
		}
	}

	return TRUE;
}



