#include "pch.h"
#include "xyzControl.h"

LRESULT CALLBACK xyzControlWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LabelEditWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

ATOM RegisterXYZControlWindowClass(HINSTANCE hInstance) {

	RegisterLableWithAditControlWindowClass(hInstance);

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
	wcex.lpszClassName = IDN_XYZ_CONTROL;
	wcex.hIconSm = nullptr;
	return RegisterClassExW(&wcex);
}

ATOM RegisterLableWithAditControlWindowClass(HINSTANCE hInstance) {
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
	wcexLabelEdit.lpszClassName = IDN_LABEL_WITH_EDIT;
	wcexLabelEdit.hIconSm = nullptr;
	return RegisterClassExW(&wcexLabelEdit);
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
			CreateWindowW(IDN_LABEL_WITH_EDIT, TEXT("X"),
						  WS_VISIBLE | WS_CHILD,
						  0, 0, sectionWidth, height,
						  hWnd, (HMENU)IDC_X, createParams->hInstance, nullptr);
			CreateWindowW(IDN_LABEL_WITH_EDIT, TEXT("Y"),
						  WS_VISIBLE | WS_CHILD,
						  sectionWidth + 5, 0, sectionWidth, height,
						  hWnd, (HMENU)IDC_Y, createParams->hInstance, nullptr);
			CreateWindowW(IDN_LABEL_WITH_EDIT, TEXT("Z"),
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
			LabelAndEditColorInfo tmp{ info->xLabelBackground,info->xLabelForeground,info->editBackground,info->editForeground };
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
			data->x = (int)(signed short)LOWORD(wParam);
			data->y = (int)(signed short)HIWORD(wParam);
			data->z = (int)(signed short)LOWORD(lParam);
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
				data->x = (int)(signed short)LOWORD(wParam);
				break;
			case IDC_Y:
				data->y = (int)(signed short)LOWORD(wParam);
				break;
			case IDC_Z:
				data->z = (int)(signed short)LOWORD(wParam);
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

struct EditRangeData {
	short max = 0;
	short min = 0;
};

LRESULT CALLBACK EditSubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

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
			EditRangeData* data = new EditRangeData();
			SetWindowSubclass(edit, EditSubClassProc, 0, (DWORD_PTR)data);


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
		SendMessage(GetDlgItem(hWnd, IDC_EDIT), UDM_SETRANGE, 0, lParam);
		break;
	case XYZ_SET_COLOR:
		{
			ColorDataMultiple* data = (ColorDataMultiple*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			LabelAndEditColorInfo* info = (LabelAndEditColorInfo*)wParam;
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

LRESULT CALLBACK EditSubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
	static int count;
	switch (uMsg) {
	case WM_NCDESTROY:
		delete (EditRangeData*)dwRefData;
		RemoveWindowSubclass(hWnd, EditSubClassProc, uIdSubclass);
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
	case WM_KEYDOWN:
		{
			BOOL tmp;
			short data;
			switch (wParam) {
			case VK_UP:
				data = GetDlgItemInt(GetParent(hWnd), IDC_EDIT, &tmp, TRUE);
				if (data == ((EditRangeData*)dwRefData)->max) data = ((EditRangeData*)dwRefData)->min;
				SetDlgItemInt(GetParent(hWnd), IDC_EDIT, (UINT)(data + 1), TRUE);
				count++;
				if (count == 2) {
					SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(0, EN_CHANGE), 0);
					count = 0;
				}
				break;
			case VK_DOWN:
				data = GetDlgItemInt(GetParent(hWnd), IDC_EDIT, &tmp, TRUE);
				if (data == ((EditRangeData*)dwRefData)->min) data = ((EditRangeData*)dwRefData)->max;
				SetDlgItemInt(GetParent(hWnd), IDC_EDIT, (UINT)(data - 1), TRUE);
				count++;
				if (count == 2) {
					SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(0, EN_CHANGE), 0);
					count = 0;
				}
				break;
			}
			return 0;
		}
	case WM_MOUSEWHEEL:
		{
			BOOL tmp;
			short data = GetDlgItemInt(GetParent(hWnd), IDC_EDIT, &tmp, TRUE);
			data += (((short)HIWORD(wParam) > 0) ? 10 : -10);
			if (data > ((EditRangeData*)dwRefData)->max) data = ((EditRangeData*)dwRefData)->min;
			else if (data < ((EditRangeData*)dwRefData)->min) data = ((EditRangeData*)dwRefData)->max;
			SetDlgItemInt(GetParent(hWnd), IDC_EDIT, (UINT)data, TRUE);
			SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(0, EN_CHANGE), 0);
			return 0;
		}
	case UDM_SETRANGE:
		{
			((EditRangeData*)dwRefData)->min = LOWORD(lParam);
			((EditRangeData*)dwRefData)->max = HIWORD(lParam);
			return 0;
		}
	case WM_KEYUP:
		{			
			if (wParam == VK_UP || wParam == VK_DOWN) {
				SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(0, EN_CHANGE), 0);
				count = 0;
			}
		}
		break;
	case WM_PAINT:
		{
			HDC hdc;
			RECT rect;
			HFONT hFont;
			RECT dimensions;
			TEXTMETRIC	textMetric;

			hdc = GetDC(hWnd);
			GetClientRect(hWnd, &dimensions);
			int height = dimensions.bottom - dimensions.top;
			hFont = (HFONT)SendMessage(hWnd, WM_GETFONT, 0, 0);
			SelectFont(hdc, hFont);
			GetTextMetrics(hdc, &textMetric);
			rect = { 0, (height - textMetric.tmHeight) / 2, (dimensions.right - dimensions.left), height - (height - textMetric.tmHeight) / 2 };
			SendMessage(hWnd, EM_SETRECT, 0, (LPARAM)&rect);
			ReleaseDC(hWnd, hdc);
		}
		break;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
