
#pragma once
#include "main.h"


class EnemyManager :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// 敵の目の前にオブジェを生成します(親がいる場合)
	void SetParentForwardObj(Actor* setObj);
	// 親のステータスの初期化をします
	void InitStatus();
	// 敵のステータスのリセットします
	void ResetStatus();
	// サウンドを再生します
	void EnemyPlaySound(const std::string soundName);
	// プレイヤーと指定されたオブジェの位置との距離を計算して返します
	float GetPlayerDistance(Actor* playerObj, Actor* otherObj);
	// デルタタイムを取得します
	float GetEnemyDeltaTime(float framerate);

private:
	//メンバ変数
	XMVECTOR mInitParentPosition;					// 親の初期位置
	XMVECTOR mInitParentRotate;;					// 親の初期の向き
	Actor* mEnemyObj;
	Actor* mEnemyParentObj;
};