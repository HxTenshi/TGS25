
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
	virtual void SearchMove()override;
	virtual void PlayerChase()override;

private:
	//メンバ変数
	SERIALIZE
	float mSpeed;				// 移動スピード
	SERIALIZE
	int mMoveInterval;			// 移動間隔
	int mInitMoveInterval;		// 初期の移動間隔
	SERIALIZE
	int mWaitInterval;			// 待機時間
	int mInitWaitInterval;		// 初期の待機時間
	int mSetDamage;
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