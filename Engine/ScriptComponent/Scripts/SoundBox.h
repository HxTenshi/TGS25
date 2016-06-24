#pragma once
#include "main.h"

class SoundBox :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// �T�E���h�̖��O��ݒ肵�܂�
	void SetSoundName(const std::string name);

private:
	//�����o�ϐ�
	bool mIsSoundPlay;
	std::string mName;
};