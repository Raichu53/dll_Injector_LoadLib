#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
//GH guide 
//working great !!!
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

int main() {
	//path to the dll file
	const char* dllPath = "C:\\VS2022\\dllfile-master\\Debug\\dllfile.dll";
	DWORD procId = 0;
	const char* procName = "ac_client.exe";
	while (!procId) {
		procId = GetProcID(procName);
		Sleep(30);
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
	return 0;
}