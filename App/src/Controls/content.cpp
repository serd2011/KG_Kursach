#include "pch.h"
#include "content.h"

#include "stuff/stuff.h"

LRESULT CALLBACK contentWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

ATOM RegisterContentWindowClass(HINSTANCE hInstance) {
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
	wcexContent.lpszClassName = IDN_CONTENT;
	wcexContent.hIconSm = nullptr;
	return RegisterClassExW(&wcexContent);
}

struct Data {
	int prevX;
	int prevY;
};

LRESULT CALLBACK contentWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_NCCREATE:
		{
			Data* data = new Data();
			if (data == nullptr) return FALSE;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)data);
			return TRUE;
		}
		break;
	case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hWnd, &rect);
			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;

			stuff::changeDimensions(width, height);
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			stuff::draw(hdc);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_MOUSEMOVE:
		{
			if ((wParam & MK_LBUTTON) || (wParam & MK_MBUTTON)) {
				Data* data = (Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
				RECT rect;
				GetClientRect(hWnd, &rect);
				int width = rect.right - rect.left;
				int height = rect.bottom - rect.top;
				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);
				int dx = x - data->prevX;
				int dy = y - data->prevY;
				data->prevX = x;
				data->prevY = y;
				double dxRel = (double)dx / (double)width;
				double dyRel = (double)dy / (double)height;
				stuff::changeCamera(dxRel, dyRel, (bool)(wParam & MK_LBUTTON));				
				SendMessage(GetParent(hWnd), CONTENT_REQUEST_REDRAW, 0, 0);				
			}
		}
		break;
	case WM_MBUTTONDOWN:
	case WM_LBUTTONDOWN:
		{
			Data* data = (Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			data->prevX = GET_X_LPARAM(lParam);
			data->prevY = GET_Y_LPARAM(lParam);
		}
		break;
	case WM_DESTROY:
		{
			Data* data = (Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			delete data;
			PostQuitMessage(0);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
