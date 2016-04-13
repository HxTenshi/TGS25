

#define _EXPORTING
#include "Input.h"


//static
int InputManager::mKeyCoord[(int)KeyCoord::Count];
bool InputManager::mMouseBool[(int)MouseCoord::Count];
int InputManager::mMouse[(int)MouseCoord::Count];
int InputManager::mMouseX;
int InputManager::mMouseY;
int InputManager::mMouseLClickX;
int InputManager::mMouseLClickY;

bool InputManager::mDIKeyboardDeviceLost;

LPDIRECTINPUT8			InputManager::pDInput = NULL;			// DirectInput�I�u�W�F�N�g
LPDIRECTINPUTDEVICE8	InputManager::pDIKeyboard = NULL;		// �L�[�{�[�h�f�o�C�X
BYTE					InputManager::diKeyState[256];		// �L�[�{�[�h���


//static
void InputManager::InitDirectInput(HWND hWnd, HINSTANCE hInst){

	mDIKeyboardDeviceLost = false;
	mMouseX = 0;
	mMouseY = 0;
	for (int i = 0; i < (int)MouseCoord::Count; i++){
		mMouse[i] = 0;
		mMouseBool[i] = false;
	}
	for (int i = 0; i < (int)KeyCoord::Count; i++){
		mKeyCoord[i] = 0;
	}

	//LPDIRECTINPUTDEVICE8	pDIJoypad = NULL;			// �W���C�p�b�h�f�o�C�X
	//DWORD					diJoyState = 0x00000000L;	// �W���C�p�b�h���
	//LPDIRECTINPUTDEVICE8	pDIMouse = NULL;			// �}�E�X�f�o�C�X
	//DIMOUSESTATE			diMouseState;				// �}�E�X���

	HRESULT	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDInput, NULL);
	if (FAILED(hr))
		return;	// DirectInput8�̍쐬�Ɏ��s

	//�L�[�{�[�h�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
	hr = pDInput->CreateDevice(GUID_SysKeyboard, &pDIKeyboard, NULL);
	if (FAILED(hr))
		return;  // �f�o�C�X�̍쐬�Ɏ��s

	//�L�[�{�[�h�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	hr = pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		return; // �f�o�C�X�̍쐬�Ɏ��s

	//���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)){
		return; // ���[�h�̐ݒ�Ɏ��s
	}

	//�L�[�{�[�h���͐���J�n
	pDIKeyboard->Acquire();

	return;
}

void InputManager::Release(){
	//���͂���ĂȂ���ԂŁH�J�����Ȃ��Ƃ����Ȃ��HSAFE_REREASE
	if (pDIKeyboard)pDIKeyboard->Release();
	if (pDInput)pDInput->Release();
}

void InputManager::Update(bool TargetFocus){

	for (int i = 0; i < (int)MouseCoord::Count; i++){
		if (mMouseBool[i]){
			mMouse[i]++;
		}
		else if (mMouse[i]>0){
			mMouse[i] = -1;
		}
		else{
			mMouse[i] = 0;
		}
	}


	//pDIKeyboard->Acquire();
	HRESULT			hr;
	hr = pDIKeyboard->GetDeviceState(256, diKeyState);
	//DIERR_INPUTLOST
	if (hr != S_OK){
		mDIKeyboardDeviceLost = true;
	}
	if (mDIKeyboardDeviceLost){
		if (pDIKeyboard->Acquire() == S_OK){
			mDIKeyboardDeviceLost = false;
		}
	}
	for (int i = 0; i < (int)KeyCoord::Count; i++){

		if (diKeyState[i] && TargetFocus){
			mKeyCoord[i]++;
			diKeyState[i] = 0;
		}
		else if (mKeyCoord[i]>0){
			mKeyCoord[i] = -1;
		}
		else{
			mKeyCoord[i] = 0;
		}
	}
}

void InputManager::SetMouseP(const LPARAM& lParam){
	mMouseX = LOWORD(lParam);	// �}�E�X�w���W�擾
	mMouseY = HIWORD(lParam);	// �}�E�X�x���W�擾
}
void InputManager::SetMouseXY(int x, int y){
	mMouseX = x;
	mMouseY = y;
}
void InputManager::SetMouseL(bool donw){
	if (mMouseBool[(int)MouseCoord::Left] != donw){
		mMouseLClickX = mMouseX;
		mMouseLClickY = mMouseY;
	}
	mMouseBool[(int)MouseCoord::Left] = donw;
}
void InputManager::SetMouseR(bool donw){
	mMouseBool[(int)MouseCoord::Right] = donw;
}