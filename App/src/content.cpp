#include "pch.h"
#include "content.h"

#include "config.h"

LRESULT CALLBACK contentWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

ATOM RegisterContentWindowClass(HINSTANCE hInstance, LPCWSTR name) {
	WNDCLASSEXW wcexContent;
	wcexContent.cbSize = sizeof(WNDCLASSEX);
	wcexContent.style = CS_HREDRAW | CS_VREDRAW;
	wcexContent.lpfnWndProc = contentWndProc;
	wcexContent.cbClsExtra = 0;
	wcexContent.cbWndExtra = 0;
	wcexContent.hInstance = hInstance;
	wcexContent.hIcon = nullptr;
	wcexContent.hCursor = LoadCursor(nullptr, IDC_ARROW);
	HBRUSH backgroundBrushContent = CreateSolidBrush(config::content::background);
	wcexContent.hbrBackground = backgroundBrushContent;
	wcexContent.lpszMenuName = nullptr;
	wcexContent.lpszClassName = name;
	wcexContent.hIconSm = nullptr;
	return RegisterClassExW(&wcexContent);
}

LRESULT contentWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// Рисование фигур
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
