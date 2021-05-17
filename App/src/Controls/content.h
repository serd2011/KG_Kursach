#pragma once
#include "pch.h"

#define IDN_CONTENT TEXT("CONTENT")
#define CONTENT_REQUEST_REDRAW (WM_USER + 0x0201)

ATOM RegisterContentWindowClass(HINSTANCE hInstance);
