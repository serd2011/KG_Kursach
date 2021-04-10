#include "Log/Log.h"

#include <Windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctime>
#include <chrono>

static HANDLE hConsole;
static HWND hWnd;

std::string getTimeStr();

void LOG::init() {
	FILE* fp;
	FreeConsole();
	AllocConsole();
	hWnd = GetConsoleWindow();

	ShowWindow(hWnd, SW_HIDE);
	HMENU hMenu = GetSystemMenu(hWnd, FALSE);
	DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MINIMIZEBOX);
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);
	DWORD prev_mode;
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &prev_mode);
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS | (prev_mode & ~(ENABLE_QUICK_EDIT_MODE)));

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	freopen_s(&fp, "CONIN$", "r", stdin);
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	setvbuf(stdout, NULL, _IONBF, 0);
}

void LOG::hide() {
	ShowWindow(hWnd, SW_HIDE);
}

void LOG::show() {
	ShowWindow(hWnd, SW_SHOWNOACTIVATE);
}

void LOG::info(char* format, ...) {
	SetConsoleTextAttribute(hConsole, 0 | 0xf);
	printf("[%s] ", getTimeStr().c_str());
	va_list argptr;
	va_start(argptr, format);
	vprintf(format, argptr);
	va_end(argptr);
	printf("\n");
}

std::string getTimeStr() {
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string s(30, '\0');
	tm t;
	localtime_s(&t, &now);
	std::strftime(&s[0], s.size(), "%H:%M:%S", &t);
	return s;
}
