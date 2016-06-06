
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
	// �v���C���[�Ƃ̋������v�Z���܂�
	void PlayerDistance(Actor* playerObj);
	// �v���C���[�ƍ��G�͈͂̎n�_�Ƃ̋�����Ԃ��܂�
	float GetPlayerDistance();
	// �v���C���[�ɓ�����������Ԃ��܂�
	bool IsPlayerSearch();
	// �v���C���[��ǐՒ��~���鋗���ɉ��Z���܂�
	void AddChaseStopDistance(float distance);

private:
	//�����o�ϐ�
	float mSizeZ;
	float mScalarZ;
	float mPlayerDistance;
	float mChaseStopDistance;
	bool mIsPlayerHit;
	bool mIsWallHit;
	Actor* wallObj;
	Actor* playerObj;
};