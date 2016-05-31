#pragma once

#include <string>
#include <vector>
#include <Windows.h>
#include "XNAMath/xnamath.h"

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
};
struct SimpleVertexNormal
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT3 Binormal;
	XMFLOAT3 Tangent;
	XMFLOAT2 Tex;
};
struct SimpleBoneVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
	UINT BoneIndex[4];
	UINT BoneWeight[4];
};
struct SimpleBoneVertexNormal
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT3 Binormal;
	XMFLOAT3 Tangent;
	XMFLOAT2 Tex;
	UINT BoneIndex[4];
	float BoneWeight[4];
};


struct IkLink
{
	int idxBone;
	unsigned char bLimit;
	float Min[3];//bLimit=1
	float Max[3];//bLimit=1
};
struct OutputSimpleBone{
	float bone_head_pos[3]; // x, y, z // �{�[���̃w�b�h�̈ʒu
	int parent_bidx;	//�{�[��Index�T�C�Y �e�{�[���ԍ�(�Ȃ��ꍇ��0xFFFF)
};
struct OutputIKBone{
	float bone_head_pos[3]; // x, y, z // �{�[���̃w�b�h�̈ʒu
	int parent_bidx;	//�{�[��Index�T�C�Y �e�{�[���ԍ�(�Ȃ��ꍇ��0xFFFF)
	int Hierarchy;//�ό`�K�w
	unsigned short bone_flag;		// �{�[���̎��

	float JointPos[3];	//BIT_JOINT=0
	int idxJoint;		//BIT_JOINT=1
	int idxGive;		//BIT_GIVE_ROT=1 || BIT_GIVE_MOVE=1
	float fGive;		//BIT_GIVE_ROT=1 || BIT_GIVE_MOVE=1
	float FixAxis[3];	//BIT_FIX_AXIS=1
	float AxisX[3];	//BIT_LOCAL_AXIS=1
	float AxisZ[3];	//BIT_LOCAL_AXIS=1
	int iExtParent;	//BIT_EXT_PARENT=1

	int t_ik_data_idx;// �{�[��Index�T�C�Y | IK�^�[�Q�b�g�{�[���̃{�[��Index;
	int t_ik_data_Loop;
	float t_ik_data_LimitAng;
};
struct SimpleBone{
	float bone_head_pos[3]; // x, y, z // �{�[���̃w�b�h�̈ʒu
	int parent_bidx;	//�{�[��Index�T�C�Y �e�{�[���ԍ�(�Ȃ��ꍇ��0xFFFF)
	int Hierarchy;//�ό`�K�w
	unsigned short bone_flag;		// �{�[���̎��

	float JointPos[3];	//BIT_JOINT=0
	int idxJoint;		//BIT_JOINT=1
	int idxGive;		//BIT_GIVE_ROT=1 || BIT_GIVE_MOVE=1
	float fGive;		//BIT_GIVE_ROT=1 || BIT_GIVE_MOVE=1
	float FixAxis[3];	//BIT_FIX_AXIS=1
	float AxisX[3];	//BIT_LOCAL_AXIS=1
	float AxisZ[3];	//BIT_LOCAL_AXIS=1
	int iExtParent;	//BIT_EXT_PARENT=1

	int t_ik_data_idx;// �{�[��Index�T�C�Y | IK�^�[�Q�b�g�{�[���̃{�[��Index;
	int t_ik_data_Loop;
	float t_ik_data_LimitAng;
	std::vector<IkLink> IKLink;
};

struct PMDVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
	UINT BoneIndex[4];
	UINT BoneWeight[4];
};

// �K�w�\��
class Hierarchy
{
public:
	Hierarchy() : mIdxSelf((UINT)-1), mIdxParent((UINT)-1){}
	UINT mIdxSelf;		// �{�[��idx
	UINT mIdxParent;	// �e�K�widx
};

// �{�[��
class Bone
{
public:
	Bone() : mScale(1, 1, 1), mRot(0, 0, 0, 1), mPos(1, 1, 1){}
	std::string mStrName;
	XMFLOAT3 mScale;	// �X�P�[��xyz
	XMFLOAT4 mRot;		// ��]�i�N�H�[�^�j�I���j
	XMFLOAT3 mPos;		// �ʒu
	XMMATRIX mMtxPose;	//
	XMMATRIX mMtxPoseInit;	//
	//WORD mIkBoneIdx;
	Hierarchy mHierarchy;
private:
};

//IK
struct Ik
{
	UINT bone_index; // IK�{�[���ԍ�
	UINT target_bone_index; // IK�^�[�Q�b�g�{�[���ԍ� // IK�{�[�����ŏ��ɐڑ�����{�[��
	UINT chain_length; // IK�`�F�[���̒���(�q�̐�)
	UINT iterations; // �ċA���Z�� // IK�l1
	float control_weight; // IK�̉e���x // IK�l2 // ���Z1�񂠂���̐����p�x
	std::vector<UINT> child_bone_index;//[ik_chain_length] IK�e�����̃{�[���ԍ�
};



#include "../Loader/Animation/VMD.h"

class KeyFrame{
public:
	KeyFrame(const vmd::VMDKeyFrame& key) :mKeyFrame(key){}
	vmd::VMDKeyFrame mKeyFrame;
	bool operator<(const KeyFrame& o){ return mKeyFrame.FrameNo < o.mKeyFrame.FrameNo; }
private:
};

//class Motion{
//public:
//	std::vector<KeyFrame> mKeyFrame;
//private:
//};