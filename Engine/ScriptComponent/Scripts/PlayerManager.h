
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
	// サウンドボックスを再生します(ステージ)
	void StageSoundBox(const std::string name);
	// サウンドボックスを再生します(プレイヤー)
	void PlayerSoundBox(const std::string name);
	// 指定したサウンドボックスを再生します
	void PlaySoundBox(Actor* soundBox, std::string name);
	// フォントの生成関数です
	void CreateFontObj(const std::string name, Actor* obj);
	void FadeFontObj();
	bool IsGameStart();

private:
	void GameStart();
	void GameClear();
	void GameOver();
	void WingUI();
	bool IsClear();
	bool GameEnd();

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
	float mGameStartTime;
	float mFadeAlpha;			// フェードの透明度
	float mStageNumberAlpha;	// ステージナンバーの透明値
	float mGatherAlpha;
	// ステージ名
	SERIALIZE
	std::string mStageName;
	SERIALIZE
	std::string mNextStageName;	// クリア時の次に移動するシーン名
	bool mIsWarning;
	bool mIsWarningSign;
	bool mIsFont;
	Actor* mFadeOutObj;
	Actor* mStageNumberObj;
	Actor* mGatherObj;
	Actor* mStageSoundBox;
	Actor* mPlayerSoundBox;
};