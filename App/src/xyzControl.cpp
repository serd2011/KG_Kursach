#include "pch.h"
#include "xyzControl.h"

#define CONTROL_LABEL_WITH_EDIT TEXT("XYZ Label Edit")

struct LabelAditColorInfo {
	COLORREF labelBackground;
	COLORREF labelForegound;
	COLORREF editBackground;
	COLORREF editForegound;
};

LRESULT CALLBACK xyzControlWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LabelEditWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

ATOM RegisterXYZControlWindowClass(HINSTANCE hInstance) {
	WNDCLASSEXW wcexLabelEdit;
	wcexLabelEdit.cbSize = sizeof(WNDCLASSEX);
	wcexLabelEdit.style = CS_HREDRAW | CS_VREDRAW;
	wcexLabelEdit.lpfnWndProc = LabelEditWndProc;
	wcexLabelEdit.cbClsExtra = 0;
	wcexLabelEdit.cbWndExtra = 0;
	wcexLabelEdit.hInstance = hInstance;
	wcexLabelEdit.hIcon = nullptr;
	wcexLabelEdit.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcexLabelEdit.hbrBackground = (HBRUSH)0;
	wcexLabelEdit.lpszMenuName = nullptr;
	wcexLabelEdit.lpszClassName = CONTROL_LABEL_WITH_EDIT;
	wcexLabelEdit.hIconSm = nullptr;
	RegisterClassExW(&wcexLabelEdit);

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = xyzControlWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)0;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = CONTROL_XYZ;
	wcex.hIconSm = nullptr;
	return RegisterClassExW(&wcex);
}

#define IDC_X 0x001
#define IDC_Y 0x002
#define IDC_Z 0x003

LRESULT CALLBACK xyzControlWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_NCCREATE:
		{
			Data* data = new Data();
			if (data == nullptr) return FALSE;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)data);
			return TRUE;
		}
		break;
	case WM_CREATE:
		{
			LPCREATESTRUCT createParams = (LPCREATESTRUCT)lParam;
			int sectionWidth = (createParams->cx - 10) / 3;
			int height = createParams->cy;
			CreateWindowW(CONTROL_LABEL_WITH_EDIT, TEXT("X"),
						  WS_VISIBLE | WS_CHILD,
						  0, 0, sectionWidth, height,
						  hWnd, (HMENU)IDC_X, createParams->hInstance, nullptr);
			CreateWindowW(CONTROL_LABEL_WITH_EDIT, TEXT("Y"),
						  WS_VISIBLE | WS_CHILD,
						  sectionWidth + 5, 0, sectionWidth, height,
						  hWnd, (HMENU)IDC_Y, createParams->hInstance, nullptr);
			CreateWindowW(CONTROL_LABEL_WITH_EDIT, TEXT("Z"),
						  WS_VISIBLE | WS_CHILD,
						  (sectionWidth + 5) * 2, 0, sectionWidth, height,
						  hWnd, (HMENU)IDC_Z, createParams->hInstance, nullptr);
		}
		break;
	case UDM_SETRANGE:
		SendMessage(GetDlgItem(hWnd, IDC_X), UDM_SETRANGE, 0, lParam);
		SendMessage(GetDlgItem(hWnd, IDC_Y), UDM_SETRANGE, 0, lParam);
		SendMessage(GetDlgItem(hWnd, IDC_Z), UDM_SETRANGE, 0, lParam);
		break;
	case XYZ_SET_COLOR:
		{
			ColorInfo* info = (ColorInfo*)wParam;
			LabelAditColorInfo tmp{ info->xLabelBackground,info->xLabelForeground,info->editBackground,info->editForeground };
			SendMessage(GetDlgItem(hWnd, IDC_X), XYZ_SET_COLOR, (WPARAM)&tmp, 0);
			tmp = { info->yLabelBackground,info->yLabelForeground,info->editBackground,info->editForeground };
			SendMessage(GetDlgItem(hWnd, IDC_Y), XYZ_SET_COLOR, (WPARAM)&tmp, 0);
			tmp = { info->zLabelBackground,info->zLabelForeground,info->editBackground,info->editForeground };
			SendMessage(GetDlgItem(hWnd, IDC_Z), XYZ_SET_COLOR, (WPARAM)&tmp, 0);
		}
		break;
	case XYZ_CHANGE_DATA:
		{
			Data* data = (Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			data->x = HIWORD(wParam);
			data->y = LOWORD(wParam);
			data->z = LOWORD(lParam);
			SendMessage(GetDlgItem(hWnd, IDC_X), XYZ_CHANGE_DATA, (WPARAM)data->x, 0);
			SendMessage(GetDlgItem(hWnd, IDC_Y), XYZ_CHANGE_DATA, (WPARAM)data->y, 0);
			SendMessage(GetDlgItem(hWnd, IDC_Z), XYZ_CHANGE_DATA, (WPARAM)data->z, 0);
		}
		break;
	case EN_CHANGE:
		{
			Data* data = (Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			auto idc = GetDlgCtrlID((HWND)lParam);
			switch (idc) {
			case IDC_X:
				data->x = LOWORD(wParam);
				break;
			case IDC_Y:
				data->y = LOWORD(wParam);
				break;
			case IDC_Z:
				data->z = LOWORD(wParam);
				break;
			default:
				return 0;
			};
			SendMessage(GetParent(hWnd), XYZ_CHANGE_DATA, GetDlgCtrlID(hWnd), (LPARAM)data);
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

// LABEL EDIT CONTROL

#define IDC_LABEL	 0x101
#define IDC_EDIT	 0x102
#define IDC_UPDOWN	 0x103

struct ColorData {
	COLORREF background;
	HBRUSH backgroundBrush;
	COLORREF foreground;
	ColorData() {
		this->background = 0xFFFFFF;
		this->backgroundBrush = CreateSolidBrush(0xFFFFFF);
		this->foreground = 0x000000;
	}
	~ColorData() {
		DeleteObject(this->backgroundBrush);
	};
	void setBackgroung(COLORREF color) {
		DeleteObject(this->backgroundBrush);
		this->background = color;
		this->backgroundBrush = CreateSolidBrush(color);
	}
	void setForeground(COLORREF color) {
		this->foreground = color;
	}
};

struct ColorDataMultiple {
	ColorData labelData;
	ColorData editData;
};

BOOL EnforceSignedIntegerEdit(HWND hwnd);

LRESULT CALLBACK LabelEditWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_NCCREATE:
		{
			ColorDataMultiple* data = new ColorDataMultiple();
			if (data == nullptr) return FALSE;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)data);
			return TRUE;
		}
		break;
	case WM_CREATE:
		{
			LPCREATESTRUCT createParams = (LPCREATESTRUCT)lParam;
			int width = createParams->cx;
			int height = createParams->cy;
			CreateWindowW(WC_STATIC, createParams->lpszName,
						  WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE | BS_PUSHBUTTON,
						  0, 0, height, height,
						  hWnd, (HMENU)IDC_LABEL, createParams->hInstance, nullptr);
			HWND edit = CreateWindowW(WC_EDIT, nullptr,
									  WS_VISIBLE | WS_CHILD | ES_CENTER | ES_MULTILINE,
									  height, 0, (width - height), height,
									  hWnd, (HMENU)IDC_EDIT, createParams->hInstance, nullptr);
			EnforceSignedIntegerEdit(edit);
			HWND upDown = CreateWindowW(UPDOWN_CLASS, nullptr,
										WS_CHILD | UDS_SETBUDDYINT | UDS_ARROWKEYS | UDS_WRAP,
										0, 0, 0, height,
										hWnd, (HMENU)IDC_UPDOWN, createParams->hInstance, nullptr
			);
			SendMessage(upDown, UDM_SETBUDDY, (WPARAM)edit, 0);
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			LONG idc = GetWindowLong((HWND)lParam, GWL_ID);
			HDC hdcStatic = (HDC)wParam;
			switch (idc) {
			case IDC_LABEL:
				{
					ColorDataMultiple* data = (ColorDataMultiple*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
					SetTextColor(hdcStatic, data->labelData.foreground);
					SetDCBrushColor(hdcStatic, data->labelData.foreground);
					SetBkColor(hdcStatic, data->labelData.background);
					return (LRESULT)data->labelData.backgroundBrush;
				}
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
				break;
			}
		}
		break;
	case WM_COMMAND:
		{
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
				{
					BOOL tmp;
					short data = GetDlgItemInt(hWnd, IDC_EDIT, &tmp, TRUE);
					SendMessage(GetParent(hWnd), EN_CHANGE, MAKEWPARAM(data, 0), (LPARAM)hWnd);
				}
				break;
			default:
				break;
			}
		}
		break;
	case WM_CTLCOLOREDIT:
		{
			HDC hdcStatic = (HDC)wParam;
			ColorDataMultiple* data = (ColorDataMultiple*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			SetTextColor(hdcStatic, data->editData.foreground);
			SetDCBrushColor(hdcStatic, data->editData.foreground);
			SetBkColor(hdcStatic, data->editData.background);
			return (LRESULT)data->editData.backgroundBrush;
		}
		break;
	case UDM_SETRANGE:
		SendMessage(GetDlgItem(hWnd, IDC_UPDOWN), UDM_SETRANGE, 0, lParam);
		break;
	case XYZ_SET_COLOR:
		{
			ColorDataMultiple* data = (ColorDataMultiple*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			LabelAditColorInfo* info = (LabelAditColorInfo*)wParam;
			data->labelData.setBackgroung(info->labelBackground);
			data->labelData.setForeground(info->labelForegound);
			data->editData.setBackgroung(info->editBackground);
			data->editData.setForeground(info->editForegound);
		}
		break;
	case XYZ_CHANGE_DATA:
		{
			SetDlgItemInt(hWnd, IDC_EDIT, (UINT)wParam, TRUE);
		}
		break;
	case WM_DESTROY:
		{
			ColorDataMultiple* data = (ColorDataMultiple*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			delete data;
			PostQuitMessage(0);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool IsUnicodeDigit(wchar_t ch) {
	WORD type;
	return GetStringTypeW(CT_CTYPE1, &ch, 1, &type) &&
		(type & C1_DIGIT);
}

LRESULT CALLBACK SignedIntegerSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
	switch (uMsg) {
	case WM_NCDESTROY:
		RemoveWindowSubclass(hWnd, SignedIntegerSubclassProc, uIdSubclass);
		break;

	case WM_CHAR:
		{
			DWORD start, end;
			SendMessage(hWnd, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
			wchar_t ch = (wchar_t)wParam;
			if (ch < L' ') break;                // let control character through
			else if (start == 0 && ch == L'-') break;          // let hyphen-minus through
			else if (start == 0 && ch == L'\x2212') break;     // let Unicode minus sign through
			else if (IsUnicodeDigit(ch)) break;  // let digit through
			MessageBeep(0);                      // otherwise invalid
			return 0;
		}
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

BOOL EnforceSignedIntegerEdit(HWND hwnd) {
	return SetWindowSubclass(hwnd, SignedIntegerSubclassProc, 0, 0);
}
