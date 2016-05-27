
#pragma once
#include "main.h"


class PlayerSearch :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// スカラー倍した値の取得
	void SetScalarZ(const float scalarZ);
	// プレイヤーと索敵範囲の始点との距離を返します
	float PlayerDistance();
	// プレイヤーに当たったかを返します
	bool IsPlayerSearch();

private:
	//メンバ変数
	float mSizeZ;
	float mPlayerDistance;
	float mScalarZ;
	bool mIsPlayerHit;
	bool mIsWallHit;
	Actor* wallObj;
	Actor* playerObj;
};