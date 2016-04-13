
#include "Window.h"
#include "Input/Input.h"
#include "Graphic/Font/Font.h"


UINT WindowState::mWidth = 1200;
UINT WindowState::mHeight = 800;


HMODULE Window::mhModuleWnd = NULL;
HWND Window::mhWnd = NULL;

Test::NativeFraction Window::mMainWindow_WPF;
HWND Window::mGameScreenHWND = NULL;

std::vector<const std::function<void(void*)>> Window::mWPFCollBacks;



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
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

	case WM_MYWMCOLLBACK:{
		const auto& func = Window::mWPFCollBacks[(int)wParam];
		if (func)func((void*)lParam);
		break;
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}