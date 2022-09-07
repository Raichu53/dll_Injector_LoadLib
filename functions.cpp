#include "injector.h"

DWORD GetProcID(const char* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 processInfo;
		processInfo.dwSize = sizeof(processInfo);
		if (Process32First(hSnap, &processInfo))
		{
			do
			{
				if (!_stricmp(processInfo.szExeFile, procName))
				{
					procId = processInfo.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &processInfo));
		}

	}
	CloseHandle(hSnap);
	return procId;
}
DWORD injection()
{
	//path to the dll file 
	const char* dllPath = "C:\\Users\\matt0\\source\\repos\\Raichu53\\dllmain\\Debug\\dllmain.dll";
	DWORD procId = 0;
	const char* procName = "csgo.exe";
	while (!procId) {
		procId = GetProcID(procName);
		Sleep(100);
	}
	std::cout << "PID: " << procId << "\n" << std::endl;
	//
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);
	if (hProc && hProc != INVALID_HANDLE_VALUE) {
		void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		if (loc) {
			WriteProcessMemory(hProc, loc, dllPath, strlen(dllPath) + 1, 0);
		}

		HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

		if (hThread) {
			CloseHandle(hThread);
		}
		if (hProc) {
			CloseHandle(hProc);
		}
	}
	return 1;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
	case WM_COMMAND:
		if (ID_BUTTON1 == LOWORD(wParam))
		{
			injection();
			return 0;
		}
		break;
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}