

#define _EXPORTING
#include "Input.h"


#include <windows.h>
#include <setupapi.h>
#include <hidsdi.h>
#pragma comment(lib, "hid.lib")
#pragma comment(lib, "setupapi.lib")

class DS4{
public:
	DS4()
		:m_HidHandle(NULL)
		, m_EventHandle(NULL)
		, m_ReceiveBuffer(NULL)
	{
		memset(&m_Overlapped, 0, sizeof(OVERLAPPED));
	}
	~DS4()
	{
	}
	void Initialize(HANDLE handle){
		m_HidHandle = handle;
		if (!m_HidHandle)return;
		if (m_EventHandle)return;

		m_EventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_Overlapped.hEvent = m_EventHandle;


		PHIDP_PREPARSED_DATA lpData;
		// Windows���Ǘ�����o�b�t�@�[�ցA�f�o�C�X�̏���ǂݍ��ݎ擾����B
		if (HidD_GetPreparsedData(m_HidHandle, &lpData))
		{
			HIDP_CAPS objCaps;


			// �f�o�C�X�̏����擾
			HidP_GetCaps(lpData, &objCaps);
			// �f�o�C�X���T�|�[�g����Report�̓Ǐo���A�����o���̃o�b�t�@�[�T�C�Y���擾���܂��B
			// USB�f�o�C�X�ł́A���̃T�C�Y���Œ�ł��B
			m_InputReportLength = objCaps.InputReportByteLength;
			m_OutputReportLength = objCaps.OutputReportByteLength;
			m_ReceiveBuffer = new byte[m_InputReportLength];
			memset(m_ReceiveBuffer, 0, sizeof(byte)*m_InputReportLength);
			// HidD_GetPreparsedData() �Ŏ擾�����f�[�^�̈���J�����܂��B
			HidD_FreePreparsedData(lpData);

			//DWORD BytesRead = 0;
			//ReadFile(m_HidHandle, m_ReceiveBuffer, m_InputReportLength, &BytesRead, &m_Overlapped);

		}
	}

	void Release(){
		if (m_HidHandle)CloseHandle(m_HidHandle);
		if (m_EventHandle)CloseHandle(m_EventHandle);
		if (m_ReceiveBuffer)delete[] m_ReceiveBuffer;
	}

	void Read(){

		DWORD BytesRead = 0;
		ReadFile(m_HidHandle, m_ReceiveBuffer, m_InputReportLength, &BytesRead, NULL);
		//Window::AddLog(buf);
		//return;
		//if (HasOverlappedIoCompleted(&m_Overlapped)){
		//	ReadFile(m_HidHandle, m_ReceiveBuffer, m_InputReportLength, &BytesRead, &m_Overlapped);
		//}
		//
		//
		//if (GetOverlappedResult(m_HidHandle, &m_Overlapped, &BytesRead, FALSE))
		//{
		//	std::string buf;
		//	for (int i = 0; i < m_InputReportLength; i++){
		//		buf += m_ReceiveBuffer[i];
		//	}
		//
		//	Window::AddLog(buf);
		//}
		//else
		//{
		//	if (GetLastError() != ERROR_IO_INCOMPLETE)
		//	{
		//	}
		//}
	}

	void ReadInput(BYTE* buttons){


		buttons[(int)PAD_DS4_KeyCoord::Button_UP] =
			(m_ReceiveBuffer[(int)DS4_ArrayFlag::POV] == DS4_BitFlag::Button_UP ||
			m_ReceiveBuffer[(int)DS4_ArrayFlag::POV] == DS4_BitFlag::Button_UP_RIGHT ||
			m_ReceiveBuffer[(int)DS4_ArrayFlag::POV] == DS4_BitFlag::Button_LEFT_UP
			);
		buttons[(int)PAD_DS4_KeyCoord::Button_RIGHT] =
			(m_ReceiveBuffer[(int)DS4_ArrayFlag::POV] == DS4_BitFlag::Button_RIGHT ||
			m_ReceiveBuffer[(int)DS4_ArrayFlag::POV] == DS4_BitFlag::Button_UP_RIGHT ||
			m_ReceiveBuffer[(int)DS4_ArrayFlag::POV] == DS4_BitFlag::Button_RIGHT_DOWN
			);
		buttons[(int)PAD_DS4_KeyCoord::Button_DOWN] =
			(m_ReceiveBuffer[(int)DS4_ArrayFlag::POV] == DS4_BitFlag::Button_DOWN ||
			m_ReceiveBuffer[(int)DS4_ArrayFlag::POV] == DS4_BitFlag::Button_RIGHT_DOWN ||
			m_ReceiveBuffer[(int)DS4_ArrayFlag::POV] == DS4_BitFlag::Button_DOWN_LEFT
			);
		buttons[(int)PAD_DS4_KeyCoord::Button_LEFT] =
			(m_ReceiveBuffer[(int)DS4_ArrayFlag::POV] == DS4_BitFlag::Button_LEFT ||
			m_ReceiveBuffer[(int)DS4_ArrayFlag::POV] == DS4_BitFlag::Button_DOWN_LEFT ||
			m_ReceiveBuffer[(int)DS4_ArrayFlag::POV] == DS4_BitFlag::Button_LEFT_UP
			);

		buttons[(int)PAD_DS4_KeyCoord::Button_SQUARE] =
			m_ReceiveBuffer[(int)DS4_ArrayFlag::Button] & DS4_BitFlag::Button_SQUARE;
		buttons[(int)PAD_DS4_KeyCoord::Button_CROSS] =
			m_ReceiveBuffer[(int)DS4_ArrayFlag::Button] & DS4_BitFlag::Button_CROSS;
		buttons[(int)PAD_DS4_KeyCoord::Button_CIRCLE] =
			m_ReceiveBuffer[(int)DS4_ArrayFlag::Button] & DS4_BitFlag::Button_CIRCLE;
		buttons[(int)PAD_DS4_KeyCoord::Button_TRIANGLE] =
			m_ReceiveBuffer[(int)DS4_ArrayFlag::Button] & DS4_BitFlag::Button_TRIANGLE;

		buttons[(int)PAD_DS4_KeyCoord::Button_L1] =
			m_ReceiveBuffer[(int)DS4_ArrayFlag::Button2] & DS4_BitFlag::Button_L1;
		buttons[(int)PAD_DS4_KeyCoord::Button_L2] =
			m_ReceiveBuffer[(int)DS4_ArrayFlag::Button2] & DS4_BitFlag::Button_L2;
		buttons[(int)PAD_DS4_KeyCoord::Button_L3] =
			m_ReceiveBuffer[(int)DS4_ArrayFlag::Button2] & DS4_BitFlag::Button_L3;

		buttons[(int)PAD_DS4_KeyCoord::Button_R1] =
			m_ReceiveBuffer[(int)DS4_ArrayFlag::Button2] & DS4_BitFlag::Button_R1;
		buttons[(int)PAD_DS4_KeyCoord::Button_R2] =
			m_ReceiveBuffer[(int)DS4_ArrayFlag::Button2] & DS4_BitFlag::Button_R2;
		buttons[(int)PAD_DS4_KeyCoord::Button_R3] =
			m_ReceiveBuffer[(int)DS4_ArrayFlag::Button2] & DS4_BitFlag::Button_R3;

		buttons[(int)PAD_DS4_KeyCoord::Button_OPTIONS] =
			m_ReceiveBuffer[(int)DS4_ArrayFlag::Button2] & DS4_BitFlag::Button_OPTIONS;
		buttons[(int)PAD_DS4_KeyCoord::Button_SHARE] =
			m_ReceiveBuffer[(int)DS4_ArrayFlag::Button2] & DS4_BitFlag::Button_SHARE;


		buttons[(int)PAD_DS4_KeyCoord::Button_1stTOUCH] =
			m_ReceiveBuffer[(int)DS4_ArrayFlag::Touch_1st];
		buttons[(int)PAD_DS4_KeyCoord::Button_2ndTOUCH] =
			m_ReceiveBuffer[(int)DS4_ArrayFlag::Touch_2nd];

		buttons[(int)PAD_DS4_KeyCoord::Button_LTOUCH] = 0;
		buttons[(int)PAD_DS4_KeyCoord::Button_RTOUCH] = 0;
	}

	void ReadAnalog(float* analog){

		ReadLR2Level(&analog[0], &analog[1]);
		ReadLStick(&analog[2], &analog[3]);
		ReadRStick(&analog[4], &analog[5]);
		Read1stTouch(&analog[6], &analog[7]);
		Read2ndTouch(&analog[8], &analog[9]);
		ReadAxisAngular(&analog[10], &analog[11], &analog[12]);
		ReadAxisAcceleration(&analog[13], &analog[14], &analog[15]);

	}
private:

	void ReadLStick(float *x, float *y){
		*x =
			(m_ReceiveBuffer[(int)DS4_ArrayFlag::LAnalogX] / 255.0f) * 2.0f - 1.0f;
		*y =
			(m_ReceiveBuffer[(int)DS4_ArrayFlag::LAnalogY] / 255.0f) * 2.0f - 1.0f;
	}
	void ReadRStick(float *x, float *y){
		*x =
			(m_ReceiveBuffer[(int)DS4_ArrayFlag::RAnalogX] / 255.0f) * 2.0f - 1.0f;
		*y =
			(m_ReceiveBuffer[(int)DS4_ArrayFlag::RAnalogY] / 255.0f) * 2.0f - 1.0f;
	}
	void ReadLR2Level(float *l, float *r){
		*l =
			(m_ReceiveBuffer[(int)DS4_ArrayFlag::L2_Level] / 255.0f);
		*r =
			(m_ReceiveBuffer[(int)DS4_ArrayFlag::R2_Level] / 255.0f);
	}
	void Read1stTouch(float *x, float *y){
		*x =
			(((unsigned int)m_ReceiveBuffer[(int)DS4_ArrayFlag::Touch_1stX_LOW]) +
			(((unsigned int)(m_ReceiveBuffer[(int)DS4_ArrayFlag::Touch_1stX_HIGH] & 0xf0)) << 8)
			) / 0x0fff;
		*y =
			(
			((unsigned int)m_ReceiveBuffer[(int)DS4_ArrayFlag::Touch_1stY_LOW] & 0x0f) +
			((unsigned int)(m_ReceiveBuffer[(int)DS4_ArrayFlag::Touch_1stY_HIGH]) << 4)
			) / 0x0fff;
	}
	void Read2ndTouch(float *x, float *y){
		*x =
			(((unsigned int)m_ReceiveBuffer[(int)DS4_ArrayFlag::Touch_2ndX_LOW]) +
			(((unsigned int)(m_ReceiveBuffer[(int)DS4_ArrayFlag::Touch_2ndX_HIGH] & 0xf0)) << 8)
			) / 0x0fff;
		*y =
			(
			((unsigned int)m_ReceiveBuffer[(int)DS4_ArrayFlag::Touch_2ndY_LOW] & 0x0f) +
			((unsigned int)(m_ReceiveBuffer[(int)DS4_ArrayFlag::Touch_2ndY_HIGH]) << 4)
			) / 0x0fff;
	}

	void ReadAxisAngular(float *x, float *y, float *z){
		*x =
			(((unsigned int)m_ReceiveBuffer[(int)DS4_ArrayFlag::XAxisAngularLOW]) +
			(((unsigned int)(m_ReceiveBuffer[(int)DS4_ArrayFlag::XAxisAngularHIGH])) << 8)
			) / 0xffff * 2.0f - 1.0f;
		*y =
			(((unsigned int)m_ReceiveBuffer[(int)DS4_ArrayFlag::YAxisAngularLOW]) +
			(((unsigned int)(m_ReceiveBuffer[(int)DS4_ArrayFlag::YAxisAngularHIGH])) << 8)
			) / 0xffff * 2.0f - 1.0f;
		*z =
			(((unsigned int)m_ReceiveBuffer[(int)DS4_ArrayFlag::ZAxisAngularLOW]) +
			(((unsigned int)(m_ReceiveBuffer[(int)DS4_ArrayFlag::ZAxisAngularHIGH])) << 8)
			) / 0xffff * 2.0f - 1.0f;
	}
	void ReadAxisAcceleration(float *x, float *y, float *z){
		*x =
			(((unsigned int)m_ReceiveBuffer[(int)DS4_ArrayFlag::XAxisAccelerationLOW]) +
			(((unsigned int)(m_ReceiveBuffer[(int)DS4_ArrayFlag::XAxisAccelerationHIGH])) << 8)
			) / 0xffff * 2.0f - 1.0f;
		*y =
			(((unsigned int)m_ReceiveBuffer[(int)DS4_ArrayFlag::YAxisAccelerationLOW]) +
			(((unsigned int)(m_ReceiveBuffer[(int)DS4_ArrayFlag::YAxisAccelerationHIGH])) << 8)
			) / 0xffff * 2.0f - 1.0f;
		*z =
			(((unsigned int)m_ReceiveBuffer[(int)DS4_ArrayFlag::ZAxisAccelerationLOW]) +
			(((unsigned int)(m_ReceiveBuffer[(int)DS4_ArrayFlag::ZAxisAccelerationHIGH])) << 8)
			) / 0xffff * 2.0f - 1.0f;
	}

	HANDLE m_HidHandle;
	HANDLE m_EventHandle;

	OVERLAPPED m_Overlapped;
	USHORT m_InputReportLength;
	USHORT m_OutputReportLength;
	byte* m_ReceiveBuffer;

	enum DS4_ArrayFlag{
		LAnalogX = 1,
		LAnalogY = 2,
		RAnalogX = 3,
		RAnalogY = 4,

		POV = 5,
		Button = 5,
		Button2 = 6,

		L2_Level = 8,
		R2_Level = 9,

		XAxisAngularLOW = 13,
		XAxisAngularHIGH = 14,
		YAxisAngularLOW = 15,
		YAxisAngularHIGH = 16,
		ZAxisAngularLOW = 17,
		ZAxisAngularHIGH = 18,

		XAxisAccelerationLOW = 19,
		XAxisAccelerationHIGH = 20,
		YAxisAccelerationLOW = 21,
		YAxisAccelerationHIGH = 22,
		ZAxisAccelerationLOW = 23,
		ZAxisAccelerationHIGH = 24,

		Touch_1st = 35,
		Touch_1stX_LOW = 36,
		Touch_1stX_HIGH = 37,
		Touch_1stY_LOW = 37,
		Touch_1stY_HIGH = 38,

		Touch_2nd = 35,
		Touch_2ndX_LOW = 36,
		Touch_2ndX_HIGH = 37,
		Touch_2ndY_LOW = 37,
		Touch_2ndY_HIGH = 38,
	};
	enum DS4_BitFlag{
		Button_UP = 0,
		Button_UP_RIGHT = 1,
		Button_RIGHT = 2,
		Button_RIGHT_DOWN = 3,
		Button_DOWN = 4,
		Button_DOWN_LEFT = 5,
		Button_LEFT = 6,
		Button_LEFT_UP = 7,
		Button_SQUARE = 1 << 4,
		Button_CROSS = 1 << 5,
		Button_CIRCLE = 1 << 6,
		Button_TRIANGLE = 1 << 7,
		Button_L1 = 1 << 0,
		Button_R1 = 1 << 1,
		Button_L2 = 1 << 2,
		Button_R2 = 1 << 3,
		Button_OPTIONS = 1 << 4,
		Button_SHARE = 1 << 5,
		Button_L3 = 1 << 6,
		Button_R3 = 1 << 7,

		Button_PS,
		Button_LTOUCH,
		Button_RTOUCH,
		Button_1stTOUCH,
		Button_2ndTOUCH,
	};

};


//static
int InputManager::mKeyCoord[(int)KeyCoord::Count];
int InputManager::mKeyCoordEngine[(int)KeyCoord::Count];
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
DS4* InputManager::ds4 = NULL;
BYTE InputManager::diDS4State[21];
float InputManager::mDS4Analog[16];
int InputManager::mDS4Input[21];

DS4 *getds4(void);

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
		mKeyCoordEngine[i] = 0;
	}

	for (int i = 0; i < 21; i++){
		diDS4State[i] = 0;
		mDS4Input[i] = 0;
	}
	for (int i = 0; i < 16; i++){
		mDS4Analog[i] = 0;
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


	ds4 = getds4();

	return;
}

void InputManager::Release(){
	//���͂���ĂȂ���ԂŁH�J�����Ȃ��Ƃ����Ȃ��HSAFE_REREASE
	if (pDIKeyboard)pDIKeyboard->Release();
	if (pDInput)pDInput->Release();
	if (ds4){
		ds4->Release();
		delete ds4;
	}
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
		}
		else if (mKeyCoord[i]>0){
			mKeyCoord[i] = -1;
		}
		else{
			mKeyCoord[i] = 0;
		}
		if (diKeyState[i]){
			mKeyCoordEngine[i]++;
			diKeyState[i] = 0;
		}
		else if (mKeyCoordEngine[i]>0){
			mKeyCoordEngine[i] = -1;
		}
		else{
			mKeyCoordEngine[i] = 0;
		}
	}

	ds4->Read();
	ds4->ReadInput(diDS4State);
	ds4->ReadAnalog(mDS4Analog);

	for (int i = 0; i < (int)PAD_DS4_KeyCoord::Count; i++){

		if (diDS4State[i] && TargetFocus){
			mDS4Input[i]++;
		}
		else if (diDS4State[i]>0){
			mDS4Input[i] = -1;
		}
		else{
			mDS4Input[i] = 0;
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


DS4 *getds4(void)
{
	//
	//�@�@�@HIDclass���ʎq�擾
	//
	GUID hidGuid;
	HidD_GetHidGuid(&hidGuid);
	//
	//      HID�f�o�C�X���Z�b�g�擾
	//
	HDEVINFO devinf;
	devinf = SetupDiGetClassDevs(&hidGuid, NULL, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	//Window::AddLog("device=" + std::to_string((int)devinf));
	//
	//�@�@�@�ʃf�o�C�X���̍\���̂̎擾
	//
	SP_DEVICE_INTERFACE_DATA spid;
	spid.cbSize = sizeof(spid);
	//
	for (int index = 0;; index++)
	{

		//Window::AddLog("Search");
		if (!SetupDiEnumDeviceInterfaces(devinf, NULL, &hidGuid, index, &spid))
		{
			//Window::AddLog("end");
			break;
		}

		//Window::AddLog("index=" + std::to_string(index));
		//
		//      �f�o�C�X�C���^�[�t�F�C�X�̏ڍ׏��̎擾
		//
		unsigned long size;
		SetupDiGetDeviceInterfaceDetail(devinf, &spid, NULL, 0, &size, 0);
		PSP_INTERFACE_DEVICE_DETAIL_DATA dev_det = PSP_INTERFACE_DEVICE_DETAIL_DATA(new char[size]);
		dev_det->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
		//
		SetupDiGetDeviceInterfaceDetail(devinf, &spid, dev_det, size, &size, 0);
		//Window::AddLog("device=" + std::string((char*)dev_det->DevicePath));
		//
		//      �t�@�C���n���h���̎擾
		//
		HANDLE handle = CreateFile(dev_det->DevicePath, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0/*FILE_FLAG_OVERLAPPED*/, NULL);
		if (handle == INVALID_HANDLE_VALUE)
		{
			//Window::AddLog("INVALID_HADLE");
			continue;
		}
		//
		//      VenderID��ProductID�̎擾
		//
		HIDD_ATTRIBUTES attr;
		HidD_GetAttributes(handle, &attr);
		//Window::AddLog("VenderID:" + std::to_string((int)attr.VendorID));
		//Window::AddLog("ProductID:" + std::to_string((int)attr.ProductID));

		//�x���_�[ID�F0x054C
		//�v���_�N�gID�F0x05C4
		if ((int)attr.VendorID == 1356 &&
			(int)attr.ProductID == 1476){

			//Window::AddLog("end");
			//Window::AddLog("ds4 create");
			DS4 *ds4 = new DS4();

			ds4->Initialize(handle);
			return ds4;

		}
		else{
			CloseHandle(handle);
		}
	}
	return NULL;
}
