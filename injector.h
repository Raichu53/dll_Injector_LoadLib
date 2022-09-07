#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#include <d3d9.h>
#include <d3dx9.h>


#define ID_BUTTON1	1001
#define ID_BUTTON2	1002

LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

DWORD GetProcID(const char* procName);
DWORD injection();