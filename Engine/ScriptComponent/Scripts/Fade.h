
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
	// �w�肵�����ԂŃt�F�[�h�C�����s���܂�
	void FadeIn(const float time);
	// �w�肵�����ԂŃt�F�[�h�A�E�g���s���܂�
	void FadeOut(const float time);
	// �t�F�[�h�C������������Ԃ��܂�
	bool IsFadeIn();
	// �t�F�[�h�A�E�g����������Ԃ��܂�
	bool IsFadeOut();

private:
	//�����o�ϐ�
	float mFadeInAlpha;
	float mFadeOutAlpha;
	bool mIsFadeIn;
	bool mIsFadeOut;
};