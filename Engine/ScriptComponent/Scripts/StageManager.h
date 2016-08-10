
#pragma once
#include "main.h"

class Actor;
class SceneCursor;
class Fade;

class StageManager :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// ポーズ画面の生成
	void createPause();
	// ポーズ画面の削除
	void deletePause();
	// ポーズ画面切り替え時のサウンド再生
	void playPauseSE();

private:
	//メンバ変数
	int mButtonCreateCount;
	int mPauseCount;
	SERIALIZE
	float mFadeOutSecond;
	bool mIsPause;
	SERIALIZE
	std::string mRetryScene;
	Actor* mFadeOutObj;
	SceneCursor* mCursorScript;
	Fade* mFadeOutScript;
};