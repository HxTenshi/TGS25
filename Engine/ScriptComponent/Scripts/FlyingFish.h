
#pragma once
#include "main.h"
#include "Enemy.h"

class FlyingFish : public Enemy {
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	void PlayerSearchMove()override;
	void PlayerChaseMove()override;
	void JampMove();

private:
	//�����o�ϐ�
	SERIALIZE
	int mUpInterval;
	int mUpCount;
	int mInitUpCount;
	SERIALIZE
	int mJampRestTime;
	int mInitJampRestTime;
	int mWallHitCount;
	SERIALIZE
	int mSetDamage;
	SERIALIZE
	int mSetResPawnTime;
	int mInitSetDamage;
	SERIALIZE
	int mUpPowar;
	SERIALIZE
	float mSetSpeed;
	SERIALIZE
	float mRotateInterval;
	SERIALIZE
	float mAddChaseStopDistance;
	float mFloorPosition;
	float mUpCosine;
	// ���G�͈͊֘A
	SERIALIZE
	float mSetSearchRengeScaleX;
	SERIALIZE
	float mSetSearchRengeScaleY;
	SERIALIZE
	float mSetSearchRengeScaleZ;
	// �����֘A
	SERIALIZE
	float mSetTornadoPower;
	SERIALIZE
	float mSetTornadoRotateScale;
	SERIALIZE
	float mSetAddTornadoRotateScale;
	SERIALIZE
	float mSetTornadoRotatePower;
	SERIALIZE
	float mSetTornadoUpPower;
	SERIALIZE
	float mSetTornadoInterval;
	SERIALIZE
	float mSetTornadoDistance;
	bool mIsJamp;
	bool mIsInitSet;
	bool mIsWallHit;
	//bool mIsFloorHit;
	XMVECTOR mJampVelocity;
};