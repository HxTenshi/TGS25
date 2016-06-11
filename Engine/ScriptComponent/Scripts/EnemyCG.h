
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
	// �A�j���[�V������ID��ύX���܂�
	void SetAnimationID(int id);
	// �A�j���[�V�����̃^�C���X�P�[����ύX���܂�
	void SetTimeScale(float timeScale);
	// �A�j���[�V�����̃��[�v��ύX���܂�
	void SetLoop(bool isLoop);

private:
	//�����o�ϐ�
	int mCurrentAnimationID;
	int mPastAnimationID;
	float mAnimationTimeScale;
	bool mIsAnimationLoop;
	bool mChangeStatus;
};