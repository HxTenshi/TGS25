
#include "SoundFileData.h"
#include <dsound.h>
#include "Sound/Sound.h"

void SoundFile::Release(){
	if (mBuffer){
		//mBuffer->Release();
		//mBuffer = NULL;
	}
}

void SoundFile::Create(const char* filename){
	// Wave�t�@�C���I�[�v��
	WAVEFORMATEX wFmt;
	char *pWaveData = 0;
	DWORD waveSize = 0;
	if (!openWave((CHAR*)filename, wFmt, &pWaveData, waveSize))
		return;

	DSBUFFERDESC DSBufferDesc;
	DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	DSBufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	DSBufferDesc.dwBufferBytes = waveSize;
	DSBufferDesc.dwReserved = 0;
	DSBufferDesc.lpwfxFormat = &wFmt;
	DSBufferDesc.guid3DAlgorithm = GUID_NULL;

	IDirectSoundBuffer *ptmpBuf = 0;
	SoundDevice::GetDevice()->CreateSoundBuffer(&DSBufferDesc, &ptmpBuf, NULL);
	ptmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (void**)&mBuffer);
	ptmpBuf->Release();
	if (mBuffer == 0) {
		return;
	}

	// �Z�J���_���o�b�t�@��Wave�f�[�^��������
	LPVOID lpvWrite = 0;
	DWORD dwLength = 0;
	if (DS_OK == mBuffer->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER)) {
		memcpy(lpvWrite, pWaveData, dwLength);
		mBuffer->Unlock(lpvWrite, dwLength, NULL, 0);
	}
	delete[] pWaveData; // �����͂�������Ȃ�
}

// Wave�t�@�C���I�[�v���֐�
bool SoundFile::openWave(TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize) {
	if (filepath == 0)
		return false;

	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo;

	// Wave�t�@�C���I�[�v��
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen(filepath, &mmioInfo, MMIO_READ);
	if (!hMmio)
		return false; // �t�@�C���I�[�v�����s

	// RIFF�`�����N����
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}

	// �t�H�[�}�b�g�`�����N����
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}
	DWORD fmsize = formatChunk.cksize;
	DWORD size = mmioRead(hMmio, (HPSTR)&waveFormatEx, fmsize);
	if (size != fmsize) {
		mmioClose(hMmio, 0);
		return false;
	}

	mmioAscend(hMmio, &formatChunk, 0);

	// �f�[�^�`�����N����
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}
	*ppData = new char[dataChunk.cksize];
	size = mmioRead(hMmio, (HPSTR)*ppData, dataChunk.cksize);
	if (size != dataChunk.cksize) {
		delete[] * ppData;
		return false;
	}
	dataSize = size;

	// �n���h���N���[�Y
	mmioClose(hMmio, 0);

	return true;
}

void SoundFile::Volume(float vol)const{
	//DSBVOLUME_MAX (�����Ȃ�) ���� DSBVOLUME_MIN (����)
	LONG v = (LONG)(DSBVOLUME_MIN - DSBVOLUME_MIN*vol);
	//�ύX����Active����Ȃ��Ƃ����Ȃ��H
	mBuffer->SetVolume(v);
}
void SoundFile::Play(bool loop)const{
	if (loop){
		if (mBuffer)mBuffer->Play(0, 0, DSBPLAY_LOOPING);
	}
	else{
		if (mBuffer)mBuffer->Play(0, 0, NULL);
	}
}

void SoundFile::Duplicate(const SoundFile* out)const{
	if (!out)return;
	if (out->mBuffer)return;
	SoundDevice::GetDevice()->DuplicateSoundBuffer(mBuffer, (LPDIRECTSOUNDBUFFER*)&out->mBuffer);
}



void SoundFile::Stop()const{
	if (mBuffer)mBuffer->Stop();
}

SoundFileData::SoundFileData(){
}
SoundFileData::~SoundFileData(){
}

void SoundFileData::Create(const char* filename){
	m_SoundData.Release();
	m_SoundData.Create(filename);
	m_FileName = filename;
}
void SoundFileData::FileUpdate(){
	m_SoundData.Release();
	m_SoundData.Create(m_FileName.c_str());
}