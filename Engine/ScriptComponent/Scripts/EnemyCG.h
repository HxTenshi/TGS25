
#pragma once
#include "main.h"

class EnemyCG :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// アニメーションのIDを変更します
	void SetAnimationID(int id);
	// アニメーションのタイムを変更します
	void SetAnimationTime(float time);
	// アニメーションのタイムスケールを変更します
	void SetTimeScale(float timeScale);
	// アニメーションのループを変更します
	void SetLoop(bool isLoop);
	// 現在のアニメーションのタイムを取得します
	float GetAnimationTime();

private:
	//メンバ変数
	int mCurrentAnimationID;
	int mPastAnimationID;
	float mAnimationTimeScale;
	bool mIsAnimationLoop;
	bool mIsAnimationChange;
	bool mIsChangeStatus;
};