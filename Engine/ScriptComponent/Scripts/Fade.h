
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
	// �t�F�[�h�̓����x��ݒ肵�܂�
	void SetFadeAlpha(const float alpha);
	// �w�肵�����ԂŃt�F�[�h�C�����s���܂�
	void FadeIn(const float time);
	// �w�肵�����ԂŃt�F�[�h�A�E�g���s���܂�
	void FadeOut(const float time,const float maxalpha = 1.0f);
	// �t�F�[�h�C���̒l���擾���܂�
	float GetFadeInAlpha();
	// �t�F�[�h�A�E�g�̒l���擾���܂�
	float GetFadeOutAlpha();
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