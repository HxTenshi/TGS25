#pragma once


//16�̔{���ɂȂ�悤�ɂ���

struct CBNeverChanges
{
	XMMATRIX mView;
	XMMATRIX mViewInv;
};

struct CBChangeOnResize
{
	XMMATRIX mProjection;
	XMMATRIX mProjectionInv;
};

struct CBChangesEveryFrame
{
	XMMATRIX mWorld;
	XMMATRIX mBeforeWorld;
};


struct cbChangesLight
{
	XMFLOAT4 LightVect;
	XMFLOAT4 LightColor;
};

struct cbChangesMaterial
{
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT4 Ambient;
	XMFLOAT2 TexScale;
	XMFLOAT2 HeightPower;
	XMFLOAT4 MNormaleScale;
	XMFLOAT2 MOffset;
	float EmissivePowor;
	float MNULL;
};
struct cbChangesUseTexture
{
	XMFLOAT4 UseTexture;
	XMFLOAT4 UseTexture2;
};
struct cbBoneMatrix
{
	//XMVECTOR
	XMFLOAT4 BoneMatrix[3];
};

struct CBBillboard
{
	XMMATRIX mBillboardMatrix;
};

struct cbChangesPointLight
{
	XMFLOAT4 ViewPosition;
	XMFLOAT4 Color;
	XMFLOAT4 Param;
};

struct cbChangesLightCamera
{
	XMMATRIX mLViewProjection[4];
	XMFLOAT4 mSplitPosition;
};

struct CBChangesPaticleParam
{
	//w=�����_���X�s�[�h
	XMFLOAT4 Vector;
	//w=[Sphere=0]~[Box=1]
	XMFLOAT4 Point;
	//w=��C��R
	XMFLOAT4 Rot;
	//x = min, y = max, z=���˗�, w=���C����
	XMFLOAT4 MinMaxScale;
	//x = min, y = max, z=Z-Collision, w=VeloRot&Bura
	XMFLOAT4 Time;
	XMFLOAT4 G;
	//x = num, y=impact, z=pointG-Rot
	XMFLOAT4 Param;
	XMFLOAT4 Wind;
	XMFLOAT4 SmoothAlpha;
};

struct cbGameParameter//register(b11)
{
	//x=AllCount,y=DeltaTimeCount,z=DeltaTime
	XMFLOAT4 Time;
};

struct cbNearFar
{
	float Near;
	float Far;
	XMFLOAT2 NULLnf;
};

//13
struct cbScreen
{
	XMFLOAT2 ScreenSize;
	XMFLOAT2 NULLss;
};

//10
struct cbFreeParam
{
	XMFLOAT4 free;
};

//CS
struct cbTextureSize//register(b0)
{
	XMUINT4 TextureSize;
};

struct cbHDRBloomParam// : register(b1)
{
	//x=���̐L�� pow(p, X ), y=�\���̐L��(�d��) lerp(bulr+center * X ,bulr, p ), z=�\���̐L��(�䗦) lerp(bulr+center * p ,bulr, X ), w=�\���̔����ʒu pow(center, X );
	XMFLOAT4 BloomParam;
};
