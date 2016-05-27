
#pragma once
#include "main.h"


class PlayerSearch :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// �X�J���[�{�����l�̎擾
	void SetScalarZ(const float scalarZ);
	// �v���C���[�ƍ��G�͈͂̎n�_�Ƃ̋�����Ԃ��܂�
	float PlayerDistance();
	// �v���C���[�ɓ�����������Ԃ��܂�
	bool IsPlayerSearch();

private:
	//�����o�ϐ�
	float mSizeZ;
	float mPlayerDistance;
	float mScalarZ;
	bool mIsPlayerHit;
	bool mIsWallHit;
	Actor* wallObj;
	Actor* playerObj;
};