
#pragma once
#include "main.h"

class CreateEnemyObj :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	int GetRandom(int min, int max);

private:
	//�����o�ϐ�
	Actor* mCreateObj;			// �����I�u�W�F�N�g
	int mVectorCount;
	SERIALIZE
	int mCreateCount;			// �����J�E���g�@�O�ɂȂ����琶��
	SERIALIZE
	int mCreateTimeInterval;		// �G�����̊Ԋu
	int mInitCreateTimeInterval;	// �����̐����Ԋu
	SERIALIZE
	int mCreateDelayTime;		// �G�������̃f�B���C�^�C��
	int mInitCreateDelayTime;	// �����̃f�B���C�^�C��
	//�������Ɏg�p
	//int mCreateSpace;			// �����_���������̔z�u�Ԋu
	int mRandomCreateRange;
	//�������Ɏg�p
	SERIALIZE
	float mReactionDistance;	// ��������
	bool mIsCreateObj;			// �G�𐶐��������H
	const char* createObjName;
	std::string baseStr;
};