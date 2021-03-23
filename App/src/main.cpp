﻿#include "pch.h"

#include "config.h"
#include "aside.h"
#include "content.h"

constexpr auto IDN_MAIN_WINDOW = L"MAIN";
constexpr auto IDN_ASIDE = L"ASIDE";
constexpr auto IDN_CONTENT = L"CONTENT";

#define IDC_ASIDE	0x001
#define IDC_CONTENT 0x002

static HINSTANCE hInst;
static HWND hWndAside;
static HWND hWndContent;

ATOM RegisterWindowClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	RegisterWindowClass(hInstance);
	RegisterAsideWindowClass(hInstance, IDN_ASIDE);
	RegisterContentWindowClass(hInstance, IDN_CONTENT);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

// Регистрирует класс окна.
ATOM RegisterWindowClass(HINSTANCE hInstance) {

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = IDN_MAIN_WINDOW;
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}

// Сохраняет маркер экземпляра и создает главное окно
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(IDN_MAIN_WINDOW, config::IDS_APP_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

// Обрабатывает сообщения в главном окне.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		{
			hWndAside = CreateWindowEx(WS_EX_TOPMOST, IDN_ASIDE, nullptr, WS_CHILD | WS_VISIBLE, 0, 0, config::asideWidth, 0, hWnd, (HMENU)IDC_ASIDE, hInst, nullptr);
			hWndContent = CreateWindowEx(WS_EX_TOPMOST, IDN_CONTENT, nullptr, WS_CHILD | WS_VISIBLE, config::asideWidth, 0, 0, 0, hWnd, (HMENU)IDC_CONTENT, hInst, nullptr);
		}
		break;
	case WM_SIZE:
		{
			RECT mainRect;
			GetClientRect(hWnd, &mainRect);
			int mainWidth = mainRect.right - mainRect.left;
			int mainHeight = mainRect.bottom - mainRect.top;
			RECT asideRect;
			GetClientRect(hWndAside, &asideRect);
			int asideWidth = asideRect.right - asideRect.left;
			int asideHight = asideRect.bottom - asideRect.top;

			SetWindowPos(hWndAside, nullptr, 0, 0, asideWidth, mainHeight, SWP_NOZORDER);
			SetWindowPos(hWndContent, nullptr, asideWidth, 0, (mainWidth - asideWidth), mainHeight, SWP_NOZORDER);
			RedrawWindow(hWnd, NULL, NULL, RDW_UPDATENOW);
		}
		break;
	case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = config::mainMinWidth;
			lpMMI->ptMinTrackSize.y = config::mainMinHeight;
		}
		break;
	case WM_COMMAND:
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}