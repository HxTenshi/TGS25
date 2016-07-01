
#pragma once
#include "main.h"
#include "Enemy.h"

class KillerWhale : public Enemy {
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	void PlayerSearchMove()override;
	void ShortDistanceAttack()override;
	void CenterDistanceAttack()override;
	void LongDistanceAttack()override;

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
	int mSetResPawnTime;
	SERIALIZE
	float mSetSpeed;
	SERIALIZE
	float mAddChaseStopDistance;
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
	float mSetTornadoInterval;
	SERIALIZE
	float mSetTornadoDistance;
	bool mIsShot;
	XMVECTOR mGRAVITY;
};