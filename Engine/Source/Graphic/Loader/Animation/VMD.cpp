#include "VMD.h"
#include <windows.h>
#include <stdio.h>

//�R���X�g���N�^
vmd::vmd(const char * filename){


	FILE *hFP;
	hFP = fopen(filename, "rb");
	if (hFP != 0){
		//�w�b�_�[�ǂݍ���
		fread(&mHeader, sizeof(VMDHeader), 1, hFP);

		fread(&mKeyNum, sizeof(mKeyNum), 1, hFP);
		mKeyFrame = new VMDKeyFrame[mKeyNum];
		fread(mKeyFrame, sizeof(VMDKeyFrame), mKeyNum, hFP);

		fclose(hFP);
		mLoadResult = true;
	}
	else{
		//MessageBox(NULL, "VMD fopen", "Err", MB_ICONSTOP);
		mLoadResult = false;
	}
}

//�f�X�g���N�^
vmd::~vmd(){
	if (!mLoadResult)return;
	delete[] mKeyFrame;
}