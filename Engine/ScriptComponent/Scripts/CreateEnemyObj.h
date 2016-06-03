
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
	int GetRandom(int min, int max);

private:
	//メンバ変数
	Actor* mCreateObj;			// 生成オブジェクト
	int mVectorCount;
	SERIALIZE
	int mCreateCount;			// 生成カウント　０になったら生成
	SERIALIZE
	int mCreateTimeInterval;		// 敵生成の間隔
	int mInitCreateTimeInterval;	// 初期の生成間隔
	SERIALIZE
	int mCreateDelayTime;		// 敵生成時のディレイタイム
	int mInitCreateDelayTime;	// 初期のディレイタイム
	//乱数時に使用
	//int mCreateSpace;			// ランダム生成時の配置間隔
	int mRandomCreateRange;
	//乱数時に使用
	SERIALIZE
	float mReactionDistance;	// 反応距離
	bool mIsCreateObj;			// 敵を生成したか？
	const char* createObjName;
	std::string baseStr;
};