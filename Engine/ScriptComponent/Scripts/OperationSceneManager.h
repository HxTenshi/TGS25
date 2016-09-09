
#pragma once
#include "main.h"


class OperationSceneManager :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// 指定のボタンが押されたかを返します
	bool IsButtonDown();

private:
	//メンバ変数
	SERIALIZE
	float mFadeSecond;
	bool mIsChangeScene;
	Actor* mFadeObj;
};