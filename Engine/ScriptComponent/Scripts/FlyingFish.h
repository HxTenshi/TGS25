
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
	//SERIALIZE
	int mUpCount;
	int mDownCount;
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
	float mUpPowar;
	SERIALIZE
	float mSetSpeed;
	float mRotateInterval;
	float mFloorPosition;
	bool mIsJamp;
	bool mIsInitSet;
	XMVECTOR mJampVelocity;
};