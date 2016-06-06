
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
	// プレイヤーとの距離を計算します
	void PlayerDistance(Actor* playerObj);
	// プレイヤーと索敵範囲の始点との距離を返します
	float GetPlayerDistance();
	// プレイヤーに当たったかを返します
	bool IsPlayerSearch();
	// プレイヤーを追跡中止する距離に加算します
	void AddChaseStopDistance(float distance);

private:
	//メンバ変数
	float mSizeZ;
	float mScalarZ;
	float mPlayerDistance;
	float mChaseStopDistance;
	bool mIsPlayerHit;
	bool mIsWallHit;
	Actor* wallObj;
	Actor* playerObj;
};