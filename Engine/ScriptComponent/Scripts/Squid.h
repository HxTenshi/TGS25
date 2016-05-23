
#pragma once
#include "main.h"
#include "Enemy.h"


class Squid :public Enemy{
public:
	virtual void Initialize()override;
	virtual void Start()override;
	virtual void Update()override;
	virtual void Finish()override;
	virtual void OnCollideBegin(Actor* target)override;
	virtual void OnCollideEnter(Actor* target)override;
	virtual void OnCollideExit(Actor* target)override;
	void ParentMove();

private:
	//�����o�ϐ�
	int mCreatCount;
	SERIALIZE
	float mSpeed;			// �ړ��X�s�[�h
	SERIALIZE
	int mMoveInterval;		// �ړ��Ԋu
	int mInitMoveInterval;	// �����̈ړ��Ԋu
	SERIALIZE
	int mWaitInterval;		// �ҋ@����
	int mInitWaitInterval;	// �����̑ҋ@����
	SERIALIZE
	int mSetDamage;
	XMVECTOR mSRPosition;
};