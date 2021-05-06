#pragma once
#include "pch.h"

#define CONTROL_XYZ TEXT("XYZ Control")
#define CONTROL_LABEL_WITH_EDIT TEXT("XYZ Label Edit")

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

struct LabelAndEditColorInfo {
	COLORREF labelBackground;
	COLORREF labelForegound;
	COLORREF editBackground;
	COLORREF editForegound;
};


struct Data {
	int x;
	int y;
	int z;
};

ATOM RegisterXYZControlWindowClass(HINSTANCE hInstance);
ATOM RegisterLableWithAditControlWindowClass(HINSTANCE hInstance);
