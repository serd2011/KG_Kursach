#pragma once
#include "pch.h"

#define ASIDE_REQUEST_REDRAW (WM_USER + 0x0101)

ATOM RegisterAsideWindowClass(HINSTANCE hInstance, LPCWSTR name);
