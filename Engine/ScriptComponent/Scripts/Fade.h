
#pragma once
#include "main.h"


class Fade :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// フェードの透明度を設定します
	void SetFadeAlpha(const float alpha);
	// 指定した時間でフェードインを行います
	void FadeIn(const float time);
	// 指定した時間でフェードアウトを行います
	void FadeOut(const float time,const float maxalpha = 1.0f);
	// フェードインの値を取得します
	float GetFadeInAlpha();
	// フェードアウトの値を取得します
	float GetFadeOutAlpha();
	// フェードインをしたかを返します
	bool IsFadeIn();
	// フェードアウトをしたかを返します
	bool IsFadeOut();

private:
	//メンバ変数
	float mFadeInAlpha;
	float mFadeOutAlpha;
	bool mIsFadeIn;
	bool mIsFadeOut;
};