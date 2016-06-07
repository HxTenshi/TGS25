#pragma once

//#include <windows.h>
//#include <xaudio2.h>
//#include <iostream>
//#include <vector>

#include <dsound.h>

#include "Window/Window.h"


class SoundDevice{
public:

	//SoundDevice()
	//	:mDirectSoundDevice(NULL)
	//{}

	static void Initialize(){
		// �T�E���h�f�o�C�X�쐬
		DirectSoundCreate8(NULL, &mDirectSoundDevice, NULL);
		mDirectSoundDevice->SetCooperativeLevel(Window::GetGameScreenHWND(), DSSCL_PRIORITY);
	}

	static void Release(){
		if (mDirectSoundDevice){
			mDirectSoundDevice->Release();
			mDirectSoundDevice = NULL;
		}
	}

	static IDirectSound8* GetDevice(){
		return mDirectSoundDevice;
	}

private:
	static IDirectSound8 *mDirectSoundDevice;
};

//class SoundPlayer{
//public:
//	SoundPlayer()
//		:pDS8(NULL)
//		,pDSBuffer(NULL)
//	{
//		init();
//	}
//	~SoundPlayer(){
//		if (pDSBuffer)pDSBuffer->Release();
//		if (pDS8)pDS8->Release();
//	}
//	void init()
//	{
//		// Wave�t�@�C���I�[�v��
//		WAVEFORMATEX wFmt;
//		char *pWaveData = 0;
//		DWORD waveSize = 0;
//		if (!openWave("EngineResource/bgm.wav", wFmt, &pWaveData, waveSize))
//			return;
//
//		// �T�E���h�f�o�C�X�쐬
//		DirectSoundCreate8(NULL, &pDS8, NULL);
//		pDS8->SetCooperativeLevel(Window::GetGameScreenHWND(), DSSCL_PRIORITY);
//
//		DSBUFFERDESC DSBufferDesc;
//		DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
//		DSBufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
//		DSBufferDesc.dwBufferBytes = waveSize;
//		DSBufferDesc.dwReserved = 0;
//		DSBufferDesc.lpwfxFormat = &wFmt;
//		DSBufferDesc.guid3DAlgorithm = GUID_NULL;
//
//		IDirectSoundBuffer *ptmpBuf = 0;
//		pDS8->CreateSoundBuffer(&DSBufferDesc, &ptmpBuf, NULL);
//		ptmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (void**)&pDSBuffer);
//		ptmpBuf->Release();
//		if (pDSBuffer == 0) {
//			pDS8->Release();
//			return;
//		}
//
//		// �Z�J���_���o�b�t�@��Wave�f�[�^��������
//		LPVOID lpvWrite = 0;
//		DWORD dwLength = 0;
//		if (DS_OK == pDSBuffer->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER)) {
//			memcpy(lpvWrite, pWaveData, dwLength);
//			pDSBuffer->Unlock(lpvWrite, dwLength, NULL, 0);
//		}
//		delete[] pWaveData; // �����͂�������Ȃ�
//
//		//DSBVOLUME_MAX (�����Ȃ�) ���� DSBVOLUME_MIN (����)
//		float vol = 0.7f;
//		LONG v = (LONG)(DSBVOLUME_MIN - DSBVOLUME_MIN*vol);
//		//�ύX����Active����Ȃ��Ƃ����Ȃ��H
//		pDSBuffer->SetVolume(v);
//	}
//
//	void Play(){
//		// �Đ�
//		if (pDSBuffer)pDSBuffer->Play(0, 0, DSBPLAY_LOOPING);
//	}
//
//	void Stop(){
//		if (pDSBuffer)pDSBuffer->Stop();
//	}
//	
//
//	// Wave�t�@�C���I�[�v���֐�
//	bool openWave(TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize) {
//		if (filepath == 0)
//			return false;
//
//		HMMIO hMmio = NULL;
//		MMIOINFO mmioInfo;
//
//		// Wave�t�@�C���I�[�v��
//		memset(&mmioInfo, 0, sizeof(MMIOINFO));
//		hMmio = mmioOpen(filepath, &mmioInfo, MMIO_READ);
//		if (!hMmio)
//			return false; // �t�@�C���I�[�v�����s
//
//		// RIFF�`�����N����
//		MMRESULT mmRes;
//		MMCKINFO riffChunk;
//		riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
//		mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
//		if (mmRes != MMSYSERR_NOERROR) {
//			mmioClose(hMmio, 0);
//			return false;
//		}
//
//		// �t�H�[�}�b�g�`�����N����
//		MMCKINFO formatChunk;
//		formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
//		mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
//		if (mmRes != MMSYSERR_NOERROR) {
//			mmioClose(hMmio, 0);
//			return false;
//		}
//		DWORD fmsize = formatChunk.cksize;
//		DWORD size = mmioRead(hMmio, (HPSTR)&waveFormatEx, fmsize);
//		if (size != fmsize) {
//			mmioClose(hMmio, 0);
//			return false;
//		}
//
//		mmioAscend(hMmio, &formatChunk, 0);
//
//		// �f�[�^�`�����N����
//		MMCKINFO dataChunk;
//		dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
//		mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
//		if (mmRes != MMSYSERR_NOERROR) {
//			mmioClose(hMmio, 0);
//			return false;
//		}
//		*ppData = new char[dataChunk.cksize];
//		size = mmioRead(hMmio, (HPSTR)*ppData, dataChunk.cksize);
//		if (size != dataChunk.cksize) {
//			delete[] * ppData;
//			return false;
//		}
//		dataSize = size;
//
//		// �n���h���N���[�Y
//		mmioClose(hMmio, 0);
//
//		return true;
//	}
//
//	IDirectSound8 *pDS8;
//	IDirectSoundBuffer8 *pDSBuffer;
//};
