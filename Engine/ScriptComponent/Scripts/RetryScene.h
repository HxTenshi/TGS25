
#pragma once
#include "main.h"


class RetryScene :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// ���X�^�[�g�V�[���̖��O�����܂�
	void SetRetrySceneName(const std::string name);
	// ���X�^�[�g�V�[���̖��O���擾���܂�
	std::string GetRetrySceneName();

private:
	//�����o�ϐ�
	static std::string mRetrySceneName;	// �ÓI�ȃV�[���̖��O
};