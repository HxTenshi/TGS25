
#pragma once
#include "main.h"
#include "Enemy.h"


class Squid :public Enemy{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	void SearchMove()override;
	void PlayerChase()override;

private:
	//�����o�ϐ�
	SERIALIZE
	int mMoveInterval;			// �ړ��Ԋu
	int mInitMoveInterval;		// �����̈ړ��Ԋu
	SERIALIZE
	int mWaitInterval;			// �ҋ@����
	int mInitWaitInterval;		// �����̑ҋ@����
	int mSetDamage;				// �_���[�W�O
	SERIALIZE
	int mSetResPawnTime;
	SERIALIZE
	int mBulletShotCount;
	int mInitBulletShotCount;
	SERIALIZE
	int mRecastCount;
	int mInitRecastCount;
	SERIALIZE
	float mSetSpeed;

	bool mIsPlayerHit;
	XMVECTOR mSRPosition;
};