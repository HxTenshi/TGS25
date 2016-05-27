
#pragma once
#include "main.h"
#include "Enemy.h"

class KillerWhale : public Enemy {
public:
	virtual void Initialize()override;
	virtual void Start()override;
	virtual void Update()override;
	virtual void Finish()override;
	virtual void OnCollideBegin(Actor* target)override;
	virtual void OnCollideEnter(Actor* target)override;
	virtual void OnCollideExit(Actor* target)override;
	virtual void SearchMove()override;
	virtual void ShortDistanceAttack()override;
	virtual void CenterDistanceAttack()override;
	virtual void LongDistanceAttack()override;

private:
	//ÉÅÉìÉoïœêî
	SERIALIZE
	int mBulletShotTime;
	int mInitBulletShotTime;
	SERIALIZE
	int mRecastTime;
	int mInitRecastTime;
	SERIALIZE
	int mSetDamage;
	SERIALIZE
	float mSetSpeed;
	bool mIsShot;
};