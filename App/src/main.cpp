#include "pch.h"

#include "Controls/aside.h"
#include "Controls/content.h"

#include "stuff/stuff.h"


#define IDN_MAIN_WINDOW TEXT("MAIN")

#define IDC_ASIDE	0x001
#define IDC_CONTENT 0x002

static HINSTANCE hInst;
static HWND hWndMain;
static HWND hWndAside;
static HWND hWndContent;
static bool isLogConsoleShown = false;

ATOM RegisterWindowClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Инициализация логгера
	LOG::init();
	LOG_HIDE();

	RegisterWindowClass(hInstance);
	RegisterAsideWindowClass(hInstance);
	RegisterContentWindowClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		if (msg.message == WM_KEYUP && msg.hwnd != hWndMain) SendMessage(hWndMain, WM_KEYUP, msg.wParam, msg.lParam);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

// Регистрирует класс окна.
ATOM RegisterWindowClass(HINSTANCE hInstance) {

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = IDN_MAIN_WINDOW;
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}

// Сохраняет маркер экземпляра и создает главное окно
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	hWndMain = CreateWindowW(IDN_MAIN_WINDOW, config::IDS_APP_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWndMain) return FALSE;

	ShowWindow(hWndMain, nCmdShow);
	UpdateWindow(hWndMain);

	return TRUE;
}

// Обрабатывает сообщения в главном окне.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_NCCREATE:
		{
			stuff::Scene* scene = new stuff::Scene();
			if (scene == nullptr) return FALSE;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)scene);
			return TRUE;
		}
		break;
	case WM_CREATE:
		{
			stuff::Scene* scene = (stuff::Scene*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			hWndAside = CreateWindowEx(WS_EX_TOPMOST, IDN_ASIDE, nullptr, WS_CHILD | WS_VISIBLE, 0, 0, config::asideWidth, 0, hWnd, (HMENU)IDC_ASIDE, hInst, scene);
			hWndContent = CreateWindowEx(WS_EX_TOPMOST, IDN_CONTENT, nullptr, WS_CHILD | WS_VISIBLE, config::asideWidth, 0, 0, 0, hWnd, (HMENU)IDC_CONTENT, hInst, scene);
			SendMessage(hWnd, ASIDE_RESET, 0, 0);
		}
		break;
	case WM_SIZE:
		{
			if (wParam == SIZE_MINIMIZED) {
				LOG_HIDE();
				break;
			}
			if (isLogConsoleShown) LOG_SHOW();

			RECT mainRect;
			GetClientRect(hWnd, &mainRect);
			int mainWidth = mainRect.right - mainRect.left;
			int mainHeight = mainRect.bottom - mainRect.top;
			RECT asideRect;
			GetClientRect(hWndAside, &asideRect);
			int asideWidth = asideRect.right - asideRect.left;
			int asideHight = asideRect.bottom - asideRect.top;

			SetWindowPos(hWndAside, nullptr, 0, 0, asideWidth, mainHeight, SWP_NOZORDER);
			SetWindowPos(hWndContent, nullptr, asideWidth, 0, (mainWidth - asideWidth), mainHeight, SWP_NOZORDER);
			RedrawWindow(hWnd, NULL, NULL, RDW_UPDATENOW);
		}
		break;
	case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = config::mainMinWidth;
			lpMMI->ptMinTrackSize.y = config::mainMinHeight;
		}
		break;
	case WM_KEYUP:
		if (wParam == VK_OEM_3) {
			if (isLogConsoleShown) LOG_HIDE();
			else LOG_SHOW();
			isLogConsoleShown = !isLogConsoleShown;
		}
		break;
	case WM_COMMAND:
		break;
	case ASIDE_REQUEST_REDRAW:
		RedrawWindow(hWndContent, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		break;
	case ASIDE_RESET:
		{
			stuff::Scene* scene = (stuff::Scene*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

			scene->changeFigure(0, stuff::TransfromType::Translation, config::objects::figure1::positionX, config::objects::figure1::positionY, config::objects::figure1::positionZ);
			scene->changeFigure(0, stuff::TransfromType::Rotation, config::objects::figure1::rotationX, config::objects::figure1::rotationY, config::objects::figure1::rotationZ);
			scene->changeFigure(0, stuff::TransfromType::Scale, config::objects::figure1::scaleX, config::objects::figure1::scaleY, config::objects::figure1::scaleZ);

			scene->changeFigure(1, stuff::TransfromType::Translation, config::objects::figure2::positionX, config::objects::figure2::positionY, config::objects::figure2::positionZ);
			scene->changeFigure(1, stuff::TransfromType::Rotation, config::objects::figure2::rotationX, config::objects::figure2::rotationY, config::objects::figure2::rotationZ);
			scene->changeFigure(1, stuff::TransfromType::Scale, config::objects::figure2::scaleX, config::objects::figure2::scaleY, config::objects::figure2::scaleZ);

			scene->changeLight(config::objects::light::positionX, config::objects::light::positionY, config::objects::light::positionZ);

			SendMessage(hWndAside, ASIDE_RESET, 0, 0);
			SendMessage(hWndContent, CONTENT_RESET, 0, 0);
			RedrawWindow(hWndContent, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		{
			stuff::Scene* scene = (stuff::Scene*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			delete scene;
			PostQuitMessage(0);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
