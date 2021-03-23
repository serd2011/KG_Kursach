#pragma once
#include "pch.h"

#define CONTROL_XYZ TEXT("XYZ Control")

//WPARAM - ColorInfo*
#define XYZ_SET_COLOR (WM_USER + 0x0001)
//WPARAM - id ; LPARAM - Data* (DO NOT CHANGE)
#define XYZ_CHANGE_DATA (WM_USER + 0x0002)

struct ColorInfo {
	COLORREF xLabelBackground;
	COLORREF xLabelForeground;
	COLORREF yLabelBackground;
	COLORREF yLabelForeground;
	COLORREF zLabelBackground;
	COLORREF zLabelForeground;
	COLORREF editBackground;
	COLORREF editForeground;
};

struct Data {
	int x;
	int y;
	int z;
};

ATOM RegisterXYZControlWindowClass(HINSTANCE hInstance);
