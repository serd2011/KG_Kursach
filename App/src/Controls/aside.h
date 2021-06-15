#pragma once
#include "pch.h"

#define IDN_ASIDE TEXT("ASIDE")
#define ASIDE_REQUEST_REDRAW (WM_USER + 0x0101)
#define ASIDE_RESET (WM_USER + 0x0102)

ATOM RegisterAsideWindowClass(HINSTANCE hInstance);
