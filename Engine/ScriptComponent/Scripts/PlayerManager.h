
#pragma once
#include "main.h"
#include<vector>


class PlayerManager :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;

	void CreditDown();
	void CreditUp();
	void ItemGet();
	// サウンドボックス生成関数です
	void CreateSoundBox(const std::string name);
	bool IsGameStart();

private:
	void GameStart();
	void GameClear();
	void GameOver();
	void WingUI();
	bool IsClear();

private:
	float mAlpha;
	bool mGameStart;
	//メンバ変数
	SERIALIZE
	int mCredit; //プレイヤーの残機
	SERIALIZE
	int mMaxPoint; //回収するアイテムの数
	int mPoint;    //回収したアイテムの数
 	SERIALIZE
	float mFadeOutSecond;
	SERIALIZE
	float mFadeInSecond;
	SERIALIZE
	float mTitleChangeTime;

	Actor* mFadeOutObj;
	Actor* mSoundBox;
};