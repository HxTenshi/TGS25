
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
	void SearchMove()override;
	void PlayerChase()override;
	void JampMove();

private:
	//ÉÅÉìÉoïœêî
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
	// çıìGîÕàÕä÷òA
	SERIALIZE
	float mSetSearchRengeScaleX;
	SERIALIZE
	float mSetSearchRengeScaleY;
	SERIALIZE
	float mSetSearchRengeScaleZ;
	// ó≥ä™ä÷òA
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
	float mSetTornadoDistance;
	bool mIsJamp;
	bool mIsInitSet;
	//bool mIsFloorHit;
	XMVECTOR mJampVelocity;
};