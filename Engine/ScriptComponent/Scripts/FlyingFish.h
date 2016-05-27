
#pragma once
#include "main.h"
#include "Enemy.h"

class FlyingFish : public Enemy {
public:
	virtual void Initialize()override;
	virtual void Start()override;
	virtual void Update()override;
	virtual void Finish()override;
	virtual void OnCollideBegin(Actor* target)override;
	virtual void OnCollideEnter(Actor* target)override;
	virtual void OnCollideExit(Actor* target)override;
	virtual void SearchMove()override;
	virtual void PlayerChase()override;
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
	int mInitSetDamage;
	SERIALIZE
	float mUpPowar;
	SERIALIZE
	float mSetSpeed;
	float mInitPositionY;
	float mRotateInterval;
	float mFloorPosition;
	bool mIsJamp;
	bool mIsInitSet;
	XMVECTOR mJampVelocity;
};