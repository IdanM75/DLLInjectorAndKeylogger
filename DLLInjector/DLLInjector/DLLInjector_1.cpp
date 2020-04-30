#include <iostream>		// To work with input
#include <direct.h>		// To work with files
#include <windows.h>	// To work with Windows API
#include <TlHelp32.h>	// General Functions to help us 

#include <fstream>

char* GetCurrentDir()							// Return the directory of where the application is find
{
	char* szRet = (char*)malloc(MAX_PATH);		// MAX_PATH , Maximum length of full path
	_getcwd(szRet, MAX_PATH);					// Gets the current working directory. 
	return szRet;
}

LPCTSTR SzToLPCTSTR(char* szString)							// Convert from LPCTSTR to string
{															// LPCTSTR = const char* ,EXAMPLE: const char* NAME: You can change the pointer, but not the char to which NAME points to 
	LPTSTR lpszRet;											// LPTSTR = char* , LPTSTR will convert implicitly to LPCSTR
	size_t size = strlen(szString) + 1;						// size_t is an unsigned integer type of at least 16 bit 
	lpszRet = (LPTSTR)malloc(MAX_PATH);
	mbstowcs_s(NULL, lpszRet, size, szString, _TRUNCATE);	// Converts a sequence of multibyte characters to a sequence of wide characters , 
	return lpszRet;
}

void WaitForProcessToAppear(LPCTSTR lpcszProc, DWORD dwDeley)									// The function wait for process to DLL injection
{
	int stop = 0;																				// MY CODE TO STOP THE WHILE IF ITS TAKE TO LONG
	HANDLE hSnap;																				// A handle can be anything from an integer index to a pointer to a resource in kernel space. The idea is that they provide an abstraction of a resource, so you don't need to know much about the resource itself to use it.
	PROCESSENTRY32 peProc;																		// Describes an entry from a list of the processes residing in the system address space when a snapshot was taken.
	BOOL bAppeared = FALSE;
	while (!bAppeared)
	{
		if ((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) != INVALID_HANDLE_VALUE)	// CreateToolhelp32Snapshot: Takes a snapshot of the specified processes, as well as the heaps, modules, and threads used by these processes.
		{
			peProc.dwSize = sizeof(PROCESSENTRY32);												// dwSize: The size of the structure, in bytes
			if (Process32First(hSnap, &peProc))													// Retrieves information about the first process encountered in a system snapshot.
				while (Process32Next(hSnap, &peProc) && !bAppeared)								// Retrieves information about the next process recorded in a system snapshot
					if (!lstrcmp(lpcszProc, peProc.szExeFile))									// Compares two character strings. The comparison is case-sensitive.
					{
						std::cout << "The process ID is: " << ("%s", peProc.th32ProcessID) << std::endl;
						bAppeared = TRUE;
					}
		}
		if (stop > 50)
		{
			std::cout << "To Long 5 Sec\n ";
			bAppeared = TRUE;
		}
		CloseHandle(hSnap);																		// Closes an open object handle.
		Sleep(dwDeley);
		stop += 1;
	}
}

DWORD GetProcessIdByName(LPCTSTR lpcszProc)													// Return the ID of the process by the LPCTSTR name of the process
{
	HANDLE hSnap;
	PROCESSENTRY32 peProc;
	DWORD dwRet = -1;
	if ((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) != INVALID_HANDLE_VALUE)
	{
		peProc.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnap, &peProc))
			while (Process32Next(hSnap, &peProc))
				if (!lstrcmp(lpcszProc, peProc.szExeFile))
					dwRet = peProc.th32ProcessID;											// th32ProcessID: The process identifier.
	}
	CloseHandle(hSnap);
	//std::cout << dwRet;																	// My test to see if the ID match to the process
	return dwRet;
}

BOOL InjectDll(DWORD dwPid, char* szDllPath)																					// Inject the DLL into the process
{
	DWORD dwMemSize;
	HANDLE hProc;
	LPVOID lpRemoteMem;																											// LPVOID: A pointer to any type
	LPVOID lpLoadLibrary;																							
	BOOL bRet = FALSE;
	if ((hProc = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD, FALSE, dwPid)) != NULL)			// OpenProcess: Opens an existing local process object
	{
		//std::cout << "hi1" << std::endl;
		dwMemSize = strlen(szDllPath) + 1;
		if ((lpRemoteMem = VirtualAllocEx(hProc, NULL, dwMemSize, MEM_COMMIT, PAGE_READWRITE)) != NULL)							// Reserves, commits, or changes the state of a region of memory within the virtual address space of a specified process. The function initializes the memory it allocates to zero.
		{
			//std::cout << "hi2" << std::endl;
			if (WriteProcessMemory(hProc, lpRemoteMem, (LPCVOID)szDllPath, dwMemSize, NULL))									// Writes data to an area of memory in a specified process. The entire area to be written to must be accessible or the operation fails.
			{
				//std::cout << "hi3" << std::endl;
				lpLoadLibrary = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");								// Retrieves the address of an exported function or variable from the specified dynamic-link library
				if (CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)lpLoadLibrary, lpRemoteMem, 0, NULL) != NULL)	// Creates a thread that runs in the virtual address space of another process
				{
					bRet = TRUE;
				}
				else
				{
					std::cout << "The CreateRemoteThread didn't succeed" << std::endl;
				}
			}
		}
	}
	CloseHandle(hProc);
	return bRet;
}



bool fexists(const char *filename)
{
	std::ifstream ifile(filename);
	return (bool)ifile;
}



int main()
{
	char szProc[MAX_PATH];
	char szDll[MAX_PATH];
	char* szDllPath = (char*)malloc(MAX_PATH);
	LPTSTR lpszProc = NULL;

	while (true)
	{
		std::cout << "Process: ";												// std: is a namespace , namespace is another identification for objects. cout: The standard output, normally the screen
		std::cin >> szProc;														// cin: The standard input, normally the keyboard
		std::cout << "DLL Injection: ";
		std::cin >> szDll;
		szDllPath = GetCurrentDir();
		strcat_s(szDllPath, MAX_PATH, "\\");									// strcat_s: Appends a string
		strcat_s(szDllPath, MAX_PATH, szDll);
		if (!fexists(szDllPath))
		{
			std::cout << "DLL dont exists" << std::endl;
		}
		else
		{
			std::cout << "DLL was found" << std::endl;
		}
		std::cout << "Waiting for process..." << std::endl;						// std::endl: Terminates a line and flushes the buffer.
		WaitForProcessToAppear(SzToLPCTSTR(szProc), 100);
		if (InjectDll(GetProcessIdByName(SzToLPCTSTR(szProc)), szDllPath))
		{
			std::cout << "Injection succeeded!" << std::endl;
		}
		else
		{
			std::cout << "Injection failed!" << std::endl;
		}
		std::cout << "\n";
	}
	return 0;
}


