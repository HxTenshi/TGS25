
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
	// アニメーションのタイムスケールを変更します
	void SetTimeScale(float timeScale);
	// アニメーションのループを変更します
	void SetLoop(bool isLoop);

private:
	//メンバ変数
	int mCurrentAnimationID;
	int mPastAnimationID;
	float mAnimationTimeScale;
	bool mIsAnimationLoop;
	bool mChangeStatus;
};