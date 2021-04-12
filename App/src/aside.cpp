#include "pch.h"
#include "aside.h"

#include "config.h"
#include "stuff.h"
#include "xyzControl.h"

static HWND hWndButton;

static HFONT hFontNormal;
static HFONT hFontSmall;

struct asideData {
	HBRUSH backgroungBrush;
	~asideData() {
		DeleteObject(this->backgroungBrush);
	}
};
static const asideData data{ CreateSolidBrush(config::aside::background) };
static const ColorInfo xyzColorInfo{
				config::aside::xLabelBackground,
				config::aside::xLabelForeground,
				config::aside::yLabelBackground,
				config::aside::yLabelForeground,
				config::aside::zLabelBackground,
				config::aside::zLabelForeground,
				config::aside::editBackground,
				config::aside::editForeground,
};

LRESULT CALLBACK AsideWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SetChildFont(HWND hwnd, LPARAM lParam);

ATOM RegisterAsideWindowClass(HINSTANCE hInstance, LPCWSTR name) {
	
	{
		HFONT hFontTmp = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		LOGFONT fontData;
		GetObject(hFontTmp, sizeof(LOGFONT), &fontData);
		fontData.lfHeight = -config::aside::fontSize::small;
		hFontSmall = CreateFontIndirect(&fontData);
		fontData.lfHeight = -config::aside::fontSize::normal;
		hFontNormal = CreateFontIndirect(&fontData);
		DeleteObject(hFontTmp);
	}

	RegisterXYZControlWindowClass(hInstance);

	WNDCLASSEXW wcexAside;
	wcexAside.cbSize = sizeof(WNDCLASSEX);
	wcexAside.style = CS_HREDRAW | CS_VREDRAW;
	wcexAside.lpfnWndProc = AsideWndProc;
	wcexAside.cbClsExtra = 0;
	wcexAside.cbWndExtra = 0;
	wcexAside.hInstance = hInstance;
	wcexAside.hIcon = nullptr;
	wcexAside.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcexAside.hbrBackground = data.backgroungBrush;
	wcexAside.lpszMenuName = nullptr;
	wcexAside.lpszClassName = name;
	wcexAside.hIconSm = nullptr;
	return RegisterClassExW(&wcexAside);
}

struct Padding {
	int top;
	int right;
	int bottom;
	int left;
};

struct SizeAndPos {
	int x;
	int y;
	int width;
	int height;
	Padding padding;

	int getYContent(int y = 0) {
		return this->y + this->padding.top + y;
	}
	int getXContent(int x = 0) {
		return this->x + this->padding.left + x;
	}

	int getContentWidth(int x = 0) {
		return this->width - (this->padding.left + this->padding.right) + x;
	}

};

struct GroupBox {
	HWND hWnd;
	SizeAndPos sizeAndPos;
	GroupBox(LPCWSTR title, SizeAndPos sizeAndPos, HWND parent, HMENU id, HINSTANCE hInst) {
		this->sizeAndPos = sizeAndPos;
		this->hWnd = CreateWindowW(WC_BUTTON, title,
								   WS_VISIBLE | WS_CHILD | BS_GROUPBOX | WS_GROUP,
								   this->sizeAndPos.x, this->sizeAndPos.y, this->sizeAndPos.width, this->sizeAndPos.height,
								   parent, id, hInst, nullptr);
	}
};

#define IDC_GROUPBOX_1		0x100
#define IDC_GROUPBOX_2		0x110
#define IDC_LIGHT			0x120
#define IDC_BUTTON_RESET	0x200

GroupBox createFigureGroupBox(LPWSTR title, SizeAndPos& size, int leftPos, HWND parent, HINSTANCE hInstance, int num);

LRESULT CALLBACK AsideWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		{
			LPCREATESTRUCT createParams = (LPCREATESTRUCT)lParam;
			int leftPos;
			{
				HDC hdc;
				SIZE size;

				hdc = GetDC(hWnd);				
				SelectFont(hdc, hFontNormal);
				GetTextExtentPoint32(hdc, TEXT("Положение:"), 10, &size);
				ReleaseDC(hWnd, hdc);
				leftPos = size.cx + 20;
			}

			// Фигура 1
			SizeAndPos size{ 10,10,(createParams->cx - 20),0 ,{20,10,12,10} };
			GroupBox gb1 = createFigureGroupBox(TEXT("Фигура 1"), size, leftPos, hWnd, createParams->hInstance, IDC_GROUPBOX_1);
			// Фигура 2
			SizeAndPos size2 = size;
			size2.y = size.y + size.height + 20;
			GroupBox gb2 = createFigureGroupBox(TEXT("Фигура 2"), size2, leftPos, hWnd, createParams->hInstance, IDC_GROUPBOX_2);
			// Свет
			SizeAndPos size3 = size2;
			size3.y = size2.y + size2.height + 20;
			size3.height = size3.padding.top + config::aside::elementHeight + size3.padding.bottom;
			GroupBox gb3(TEXT("Свет"), size3, hWnd, nullptr, createParams->hInstance);
			CreateWindowW(WC_STATIC, TEXT("Положение:"),
						  WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | SS_CENTERIMAGE,
						  gb3.sizeAndPos.getXContent(), gb3.sizeAndPos.getYContent(), leftPos, config::aside::elementHeight,
						  hWnd, nullptr, createParams->hInstance, nullptr);
			HWND tmp = CreateWindowW(CONTROL_XYZ, nullptr,
									 WS_VISIBLE | WS_CHILD,
									 gb3.sizeAndPos.getXContent(leftPos), gb3.sizeAndPos.getYContent(), gb3.sizeAndPos.getContentWidth(-leftPos), config::aside::elementHeight,
									 hWnd, (HMENU)IDC_LIGHT, createParams->hInstance, nullptr);
			SendMessage(tmp, UDM_SETRANGE, 0, MAKELPARAM(-5000, 5000));
			SendMessage(tmp, XYZ_SET_COLOR, (WPARAM)&xyzColorInfo, 0);
			SendMessage(tmp, XYZ_CHANGE_DATA, 0, 0);

			// Сбросить
			hWndButton = CreateWindowW(WC_BUTTON, TEXT("Сбросить"),
									   WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
									   0, 0, (createParams->cx - 20), 30,
									   hWnd, (HMENU)IDC_BUTTON_RESET, createParams->hInstance, nullptr);

			EnumChildWindows(hWnd, SetChildFont, (LPARAM)hFontNormal);
			SendMessage(gb1.hWnd, WM_SETFONT, (WPARAM)hFontSmall, MAKELONG(TRUE, 0));
			SendMessage(gb2.hWnd, WM_SETFONT, (WPARAM)hFontSmall, MAKELONG(TRUE, 0));
			SendMessage(gb3.hWnd, WM_SETFONT, (WPARAM)hFontSmall, MAKELONG(TRUE, 0));

			SendMessage(hWnd, WM_COMMAND, IDC_BUTTON_RESET, 0);
		}
		break;
	case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hWnd, &rect);
			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;

			SetWindowPos(hWndButton, nullptr, 10, (height - 10 - 40), (width - 20), 40, SWP_NOZORDER);
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, config::aside::foreground);
			SetDCBrushColor(hdcStatic, config::aside::background);
			SetBkColor(hdcStatic, config::aside::background);
			return (LRESULT)data.backgroungBrush;
		}
		break;
	case WM_COMMAND:
		switch (wParam) {
		case IDC_BUTTON_RESET:
			{
				SendMessage(GetDlgItem(hWnd, IDC_GROUPBOX_1 + 2), XYZ_CHANGE_DATA, MAKEWPARAM(config::transforms::figure1::positionX, config::transforms::figure1::positionY), MAKELPARAM(config::transforms::figure1::positionZ, 0));
				SendMessage(GetDlgItem(hWnd, IDC_GROUPBOX_1 + 3), XYZ_CHANGE_DATA, MAKEWPARAM(config::transforms::figure1::rotationX, config::transforms::figure1::rotationY), MAKELPARAM(config::transforms::figure1::rotationZ, 0));
				SendMessage(GetDlgItem(hWnd, IDC_GROUPBOX_1 + 4), XYZ_CHANGE_DATA, MAKEWPARAM(config::transforms::figure1::scaleX, config::transforms::figure1::scaleY), MAKELPARAM(config::transforms::figure1::scaleZ, 0));
				SendMessage(GetDlgItem(hWnd, IDC_GROUPBOX_2 + 2), XYZ_CHANGE_DATA, MAKEWPARAM(config::transforms::figure2::positionX, config::transforms::figure2::positionY), MAKELPARAM(config::transforms::figure2::positionZ, 0));
				SendMessage(GetDlgItem(hWnd, IDC_GROUPBOX_2 + 3), XYZ_CHANGE_DATA, MAKEWPARAM(config::transforms::figure2::rotationX, config::transforms::figure2::rotationY), MAKELPARAM(config::transforms::figure2::rotationZ, 0));
				SendMessage(GetDlgItem(hWnd, IDC_GROUPBOX_2 + 4), XYZ_CHANGE_DATA, MAKEWPARAM(config::transforms::figure2::scaleX, config::transforms::figure2::scaleY), MAKELPARAM(config::transforms::figure2::scaleZ, 0));
				SendMessage(GetDlgItem(hWnd, IDC_LIGHT), XYZ_CHANGE_DATA, MAKEWPARAM(config::transforms::light::positionX, config::transforms::light::positionY), MAKELPARAM(config::transforms::light::positionZ, 0));

				stuff::resetAll();

				SendMessage(GetParent(hWnd), ASIDE_REQUEST_REDRAW, 0, 0);
			}
		}
		break;
	case XYZ_CHANGE_DATA:
		{
			int idc = (int)wParam;
			const Data* data = (const Data*)lParam;
			int fugireNum = (idc >> 4) & 0xF;
			if (fugireNum > 2) break;
			if (fugireNum == 2) {
				stuff::changeLight(data->x, data->y, data->z);
			} else {
				int xyzNum = idc & 0xF;
				switch (xyzNum) {
				case 2:
					stuff::changeFigure(fugireNum, stuff::TransfromType::Translation, data->x, data->y, data->z);
					break;
				case 3:
					stuff::changeFigure(fugireNum, stuff::TransfromType::Rotation, data->x, data->y, data->z);
					break;
				case 4:
					stuff::changeFigure(fugireNum, stuff::TransfromType::Scale, data->x, data->y, data->z);
					break;
				}
			}
			SendMessage(GetParent(hWnd), ASIDE_REQUEST_REDRAW, 0, 0);
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

BOOL CALLBACK SetChildFont(HWND hwnd, LPARAM lParam) {
	HFONT hFont = (HFONT)lParam;
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, MAKELONG(TRUE, 0));
	return TRUE;
}

GroupBox createFigureGroupBox(LPWSTR title, SizeAndPos& size, int leftPos, HWND parent, HINSTANCE hInstance, int boxNum) {
	size.height = size.padding.top + (config::aside::elementHeight + 2) * 3 + size.padding.bottom;

	GroupBox gb(title, size, parent, nullptr, hInstance);

	HWND tmp;
	int y = gb.sizeAndPos.getYContent();
	CreateWindowW(WC_STATIC, TEXT("Положение:"),
				  WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | SS_CENTERIMAGE,
				  gb.sizeAndPos.getXContent(), y, leftPos, config::aside::elementHeight,
				  parent, nullptr, hInstance, nullptr);
	tmp = CreateWindowW(CONTROL_XYZ, nullptr,
						WS_VISIBLE | WS_CHILD,
						gb.sizeAndPos.getXContent(leftPos), y, gb.sizeAndPos.getContentWidth(-leftPos), config::aside::elementHeight,
						parent, (HMENU)((size_t)boxNum + 2), hInstance, nullptr);
	SendMessage(tmp, UDM_SETRANGE, 0, MAKELPARAM(-5000, 5000));
	SendMessage(tmp, XYZ_SET_COLOR, (WPARAM)&xyzColorInfo, 0);
	SendMessage(tmp, XYZ_CHANGE_DATA, 0, 0);


	y = gb.sizeAndPos.getYContent((config::aside::elementHeight + 2) * 1 );
	CreateWindowW(WC_STATIC, TEXT("Поворот:"),
				  WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | SS_CENTERIMAGE,
				  gb.sizeAndPos.getXContent(), y, leftPos, config::aside::elementHeight,
				  parent, nullptr, hInstance, nullptr);
	tmp = CreateWindowW(CONTROL_XYZ, nullptr,
						WS_VISIBLE | WS_CHILD,
						gb.sizeAndPos.getXContent(leftPos), y, gb.sizeAndPos.getContentWidth(-leftPos), config::aside::elementHeight,
						parent, (HMENU)((size_t)boxNum + 3), hInstance, nullptr);
	SendMessage(tmp, UDM_SETRANGE, 0, MAKELPARAM(-360, 360));
	SendMessage(tmp, XYZ_SET_COLOR, (WPARAM)&xyzColorInfo, 0);
	SendMessage(tmp, XYZ_CHANGE_DATA, 0, 0);

	y = gb.sizeAndPos.getYContent((config::aside::elementHeight + 2) * 2 );
	CreateWindowW(WC_STATIC, TEXT("Масштаб:"),
				  WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | SS_CENTERIMAGE,
				  gb.sizeAndPos.getXContent(), y, leftPos, config::aside::elementHeight,
				  parent, nullptr, hInstance, nullptr);
	tmp = CreateWindowW(CONTROL_XYZ, nullptr,
						WS_VISIBLE | WS_CHILD,
						gb.sizeAndPos.getXContent(leftPos), y, gb.sizeAndPos.getContentWidth(-leftPos), config::aside::elementHeight,
						parent, (HMENU)((size_t)boxNum + 4), hInstance, nullptr);
	SendMessage(tmp, UDM_SETRANGE, 0, MAKELPARAM(10, 500));
	SendMessage(tmp, XYZ_SET_COLOR, (WPARAM)&xyzColorInfo, 0);
	SendMessage(tmp, XYZ_CHANGE_DATA, 0, 0);

	return gb;
}
