
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