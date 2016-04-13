#pragma once

class vmd{
public:
	bool mLoadResult;
#pragma pack(push,1)	//�A���C�������g������I�t��

	struct VMDHeader
	{
		char Header[30]; // "Vocaloid Motion Data 0002"
		char ModelName[20];
	};
	VMDHeader mHeader;

	unsigned int mKeyNum;
	struct VMDKeyFrame
	{
		char BoneName[15];
		unsigned int FrameNo;//time
		float Location[3];
		float Rotatation[4]; // Quaternion
		char Interpolation[64]; // [4][4][4]
	};
	VMDKeyFrame* mKeyFrame;

#pragma pack(pop)	//�A���C�������g����G���h

	//�R���X�g���N�^
	vmd(const char *);

	//�f�X�g���N�^
	~vmd();
};