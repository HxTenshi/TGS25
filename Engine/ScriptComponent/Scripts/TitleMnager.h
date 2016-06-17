
#pragma once
#include "main.h"

class Actor;
class SceneCursor;
class Fade;

class TitleMnager :public IDllScriptComponent{
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
	float mCameraRotateSpeed;
	SERIALIZE
	float mFadeOutSecond;
	Actor* mCursorObj;
	Actor* mFadeOutObj;
	SceneCursor* mCursorScript;
	Fade*  mFadeOutScript;
};