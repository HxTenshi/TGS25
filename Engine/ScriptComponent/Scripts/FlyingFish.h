
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

private:
	//ÉÅÉìÉoïœêî
	SERIALIZE
	int mUpInterval;
	int mUpCount;
	int mDownCount;
	int mJampRestTime;
	int mInitJampRestTime;
	int mWallHitCount;
	float mUpPowar;
	float mInitPositionY;
	float mRotateInterval;
	bool mIsJamp;
	bool mIsInitSet;
	XMVECTOR mJampVelocity;
};