
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

private:
	//�����o�ϐ�
	SERIALIZE
	Actor* mCreateObj;			// �����I�u�W�F�N�g
	SERIALIZE
	int mCreateCount;			// �����J�E���g�@�O�ɂȂ����琶��
	SERIALIZE
	int mCreateInterval;		// �G�����̊Ԋu
	int mInitCreateInterval;	// �����̐����Ԋu
	SERIALIZE
	int mCreateDelayTime;		// �G�������̃f�B���C�^�C��
	int mInitCreateDelayTime;	// �����̃f�B���C�^�C��
	SERIALIZE
	float mReactionDistance;	// ��������
	bool mIsCreateObj;			// �G�𐶐��������H
};