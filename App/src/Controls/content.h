#pragma once
#include "pch.h"

#define IDN_CONTENT TEXT("CONTENT")
#define CONTENT_REQUEST_REDRAW (WM_USER + 0x0201)
#define CONTENT_RESET (WM_USER + 0x0202)

ATOM RegisterContentWindowClass(HINSTANCE hInstance);
