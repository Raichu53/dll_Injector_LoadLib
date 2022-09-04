#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#include <d3d9.h>
#include <d3dx9.h>


#define ID_BUTTON1	1001
#define ID_BUTTON2	1002

// Global variables
HWND hButton;

LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);


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
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
{
	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		"WindowClass1",    // name of the window class
		"injector",   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		200,    // x-position of the window
		200,    // y-position of the window
		300,    // width of the window
		200,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL
	hButton = CreateWindow("button", "injection", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		100, 25, 80, 30, hWnd, (HMENU)ID_BUTTON1, hInstance, NULL);
	hButton = CreateWindow("button", "test", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		100, 75, 80, 30, hWnd, (HMENU)ID_BUTTON2, hInstance, NULL);
	// display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	// enter the main loop:

	// this struct holds Windows event messages
	MSG msg;
	int count = 0;
	// Enter the infinite message loop
	while (TRUE)
	{
		// Check to see if any messages are waiting in the queue
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);
		}

		// If the message is WM_QUIT, exit the while loop
		if (msg.message == WM_QUIT)
			break;

		//code
		
	}
	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
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
		if (ID_BUTTON2 == LOWORD(wParam))
		{	
			MessageBox(NULL, "test", "test", NULL);
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