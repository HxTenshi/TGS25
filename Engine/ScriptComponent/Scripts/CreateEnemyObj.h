
#pragma once
#include "main.h"
#include <vector>

enum class EnemyName {
	FlyingFishName = 0,
	KillerWhaleName = 1,
	RandomName = 2
};

class CreateEnemyObj :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// 生成オブジェクトの名前を設定します
	void CreateName();
	// ランダム関数です
	int GetRandom(int min, int max);
	// 敵が生成オブジェクトに戻る距離を返します
	float GetReturnDistance();

private:
	//メンバ変数
	Actor* mCreateObj;				// 生成オブジェクト
	int mVectorCount;
	SERIALIZE
	int mCreateCount;				// 生成カウント
	SERIALIZE
	int mOnceCreateCount;			// 一回に生成する敵の数
	SERIALIZE
	int mCreateTimeInterval;		// 敵生成の間隔
	int mInitCreateTimeInterval;	// 初期の生成間隔
	SERIALIZE
	int mCreateDelayTime;			// 敵生成時のディレイタイム
	int mInitCreateDelayTime;		// 初期のディレイタイム
	SERIALIZE
	int mRandomCreateRange;			// ランダム生成時の配置間隔
	//乱数時に使用
	SERIALIZE
	float mReactionDistance;		// 反応距離
	SERIALIZE
	float mEnemyReturnDistance;		// 敵が生成オブジェクトに戻る距離
	bool mIsCreateObj;				// 敵を生成したか？
	SERIALIZE
	bool mIsRandom;					// 乱数を使用するか？
	bool mIsInfinityCreate;			// 無限生成にするか？
	const char* mCreateObjName;		// 生成オブジェクトの名前
	SERIALIZE
	std::string mEnemyName;			// 生成する敵の名前
	std::string mBaseStr;			// 名前の元
	// 生成オブジェクトの名前コンテナ
	typedef std::vector<std::string> NameContainer;
	NameContainer mNameContainer;
};