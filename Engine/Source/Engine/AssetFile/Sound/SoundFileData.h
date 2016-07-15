#pragma once
#include "../AssetFileData.h"

class IDirectSoundBuffer8;

class SoundFile{
public:

	SoundFile()
		:mBuffer(NULL)
	{}
	~SoundFile(){
		Release();
	}

	void Release();

	void Volume(float vol)const;
	void Play(bool loop)const;
	void Stop()const;

	void Duplicate(const SoundFile* out)const;

	bool Create(const char* filename);

private:
	// Wave�t�@�C���I�[�v���֐�
	bool openWave(TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize);

	IDirectSoundBuffer8 *mBuffer;
};

class SoundFileData : public AssetFileData{
public:
	SoundFileData();
	~SoundFileData();

	bool Create(const char* filename) override;
	bool FileUpdate() override;

	const SoundFile* GetSoundFile() const{
		return &m_SoundData;
	}

private:

	//�R�s�[�֎~
	SoundFileData(const SoundFileData&) = delete;
	SoundFileData& operator=(const SoundFileData&) = delete;

	SoundFile m_SoundData;
};