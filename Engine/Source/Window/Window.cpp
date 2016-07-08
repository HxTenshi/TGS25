
#include "Window.h"
#include "Input/Input.h"
#include "Graphic/Font/Font.h"

#include "../resource/resource.h"

#ifdef _ENGINE_MODE

#ifdef _DEBUG
#pragma comment(lib, "Debug/CppWPFdll.lib")
#else
#pragma comment(lib, "Release/CppWPFdll.lib")
#endif

#endif


UINT WindowState::mWidth = 1200;
UINT WindowState::mHeight = 800;


//HMODULE Window::mhModuleWnd = NULL;
HWND Window::mhWnd = NULL;
#ifdef _ENGINE_MODE
std::vector<const std::function<void(void*)>> Window::mWPFCollBacks;
Test::NativeFraction Window::mMainWindow_WPF;
#endif

HWND Window::mGameScreenHWND = NULL;


int Window::Init(){

#ifdef _ENGINE_MODE
	mWPFCollBacks.resize((int)MyWindowMessage::Count);
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = NULL;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = mhInstance;
	wcex.hIcon = LoadIcon(mhInstance, (LPCTSTR)IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "TenshiEngineDummyWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	RECT rc = { 0, 0, 1, 1 };
	AdjustWindowRect(&rc, NULL, FALSE);
	mDummyhWnd = CreateWindow("TenshiEngineDummyWindowClass", "TenshiEngineDummyWindow", NULL,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, mhInstance,
		NULL);
	if (!mDummyhWnd)
		return E_FAIL;
#else
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = NULL;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = mhInstance;
	wcex.hIcon = LoadIcon(mhInstance, (LPCTSTR)IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "GameWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	RECT rc = { 0, 0, WindowState::mWidth, WindowState::mHeight };
	AdjustWindowRect(&rc, NULL, FALSE);

	DWORD WindowModeFlag = WS_POPUP;
	//DWORD WindowModeFlag = WS_OVERLAPPEDWINDOW;


	mhWnd = CreateWindow("GameWindowClass", "GameWindow", 
		WindowModeFlag,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, mhInstance,
		NULL);
	mGameScreenHWND = mhWnd;
	ShowWindow(mhWnd, mnCmdShow);
#endif

	return S_OK;
}

#include "Library/Movie/Movie.h"
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	//���[�r�[
	case WM_MFSNOTIFY:
		if (wParam != NULL){ ((CMFSession *)wParam)->HandleEvent(lParam); }
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//BitBlt(hdc, 0, 0, WindowState::mWidth, WindowState::mHeight, hdc, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
		// �}�E�X���ړ������Ƃ��̏���
		InputManager::SetMouseP(lParam);
		break;

	case WM_LBUTTONDOWN:
		// �}�E�X���{�^�����N���b�N���ꂽ�Ƃ��̏���
		InputManager::SetMouseL(true);
		break;
	case WM_LBUTTONUP:
		// �}�E�X���{�^�����N���b�N�������ꂽ�Ƃ��̏���

		//�E�B���h�E�O�ŗ����ƃ_��
		InputManager::SetMouseL(false);
		break;

	case WM_RBUTTONDOWN:
		// �}�E�X�E�{�^�����N���b�N���ꂽ�Ƃ��̏���
		InputManager::SetMouseR(true);
		break;
	case WM_RBUTTONUP:
		// �}�E�X�E�{�^�����N���b�N�������ꂽ�Ƃ��̏���
		InputManager::SetMouseR(false);
		break;

	case WM_CREATE:
		break;

#ifdef _ENGINE_MODE
	case WM_MYWMCOLLBACK:{
		const auto& func = Window::mWPFCollBacks[(int)wParam];
		if (func)func((void*)lParam);
		break;
	}
#endif
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}