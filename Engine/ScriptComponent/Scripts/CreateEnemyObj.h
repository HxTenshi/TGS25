
#pragma once
#include "main.h"


class CreateEnemyObj :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;

private:
	//メンバ変数
	SERIALIZE
	Actor* mCreateObj;			// 生成オブジェクト
	SERIALIZE
	int mCreateCount;			// 生成カウント　０になったら生成
	SERIALIZE
	int mCreateInterval;		// 敵生成の間隔
	int mInitCreateInterval;	// 初期の生成間隔
	SERIALIZE
	int mCreateDelayTime;		// 敵生成時のディレイタイム
	int mInitCreateDelayTime;	// 初期のディレイタイム
	SERIALIZE
	float mReactionDistance;	// 反応距離
	bool mIsCreateObj;			// 敵を生成したか？
};