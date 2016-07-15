
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
	// 外部でプレイヤーを見つけたかの変更を行います
	void SetIsPlayerHit(bool isHit);
	// 外部でIsLostの変更を行います
	void SetIsLost(bool isLost);
	// プレイヤーと索敵範囲の始点との距離を返します
	float GetPlayerDistance();
	// プレイヤーに当たったかを返します
	bool IsPlayerSearch();
	// 追跡中にプレイヤーを見失ったかを返します
	bool IsLost();
	// プレイヤーを追跡中止する距離に加算します
	void AddChaseStopDistance(float distance);

private:
	//メンバ変数
	float mSizeZ;				// Zの大きさ
	float mScalarZ;				// Zのスカラー倍
	float mPlayerDistance;		// プレイヤーとの距離
	float mChaseStopDistance;	// プレイヤーを見失う距離
	bool mIsPlayerHit;			// プレイヤーと索敵範囲がぶつかったか？
	bool mIsWallHit;			// 壁とぶつかっているか？
	bool mIsLost;				// プレイヤーを見失ったか？
	Actor* wallObj;				// 壁オブジェクト
	Actor* playerObj;			// プレイヤーオブジェクト
};