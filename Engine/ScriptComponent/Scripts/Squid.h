
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
	//メンバ変数
	SERIALIZE
	int mMoveInterval;			// 移動間隔
	int mInitMoveInterval;		// 初期の移動間隔
	SERIALIZE
	int mWaitInterval;			// 待機時間
	int mInitWaitInterval;		// 初期の待機時間
	int mSetDamage;				// ダメージ０
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