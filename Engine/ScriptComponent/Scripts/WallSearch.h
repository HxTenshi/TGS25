
#pragma once
#include "main.h"


class WallSearch :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// �ǂɂԂ���������Ԃ��܂�
	bool IsWallHit();

private:
	// �Ԃ������I�u�W�F�N�g���ړI�̃I�u�W�F�N�g�Ȃ�ture��Ԃ��܂�
	bool IsNameHit(Actor* target);

private:
	//�����o�ϐ�
	bool mIsWallHit;
};