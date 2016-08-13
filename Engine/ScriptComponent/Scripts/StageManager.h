
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
	// �|�[�Y��ʂ̐���
	void createPause();
	// �|�[�Y��ʂ̍폜
	void deletePause();
	// �|�[�Y��ʐ؂�ւ����̃T�E���h�Đ�
	void playPauseSE();

private:
	//�����o�ϐ�
	int mButtonCreateCount;		// �{�^���J�E���g
	int mPauseCount;			// �|�[�Y�̉�
	SERIALIZE
	float mFadeOutSecond;		// �t�F�[�h�A�E�g�̎���
	SERIALIZE
	float mFadeAlpha;			// �t�F�[�h�̓����x
	bool mIsPause;				// �|�[�Y�����H
	SERIALIZE
	std::string mRetryScene;	// ���g���C����V�[���̖��O
	Actor* mFadeOutObj;			// �t�F�[�h�I�u�W�F
	SceneCursor* mCursorScript;	// �J�[�\���X�N���v�g
	Fade* mFadeOutScript;		// �t�F�[�h�X�N���v�g
};