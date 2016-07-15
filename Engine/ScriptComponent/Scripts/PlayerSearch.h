
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
	// �O���Ńv���C���[�����������̕ύX���s���܂�
	void SetIsPlayerHit(bool isHit);
	// �O����IsLost�̕ύX���s���܂�
	void SetIsLost(bool isLost);
	// �v���C���[�ƍ��G�͈͂̎n�_�Ƃ̋�����Ԃ��܂�
	float GetPlayerDistance();
	// �v���C���[�ɓ�����������Ԃ��܂�
	bool IsPlayerSearch();
	// �ǐՒ��Ƀv���C���[��������������Ԃ��܂�
	bool IsLost();
	// �v���C���[��ǐՒ��~���鋗���ɉ��Z���܂�
	void AddChaseStopDistance(float distance);

private:
	//�����o�ϐ�
	float mSizeZ;				// Z�̑傫��
	float mScalarZ;				// Z�̃X�J���[�{
	float mPlayerDistance;		// �v���C���[�Ƃ̋���
	float mChaseStopDistance;	// �v���C���[������������
	bool mIsPlayerHit;			// �v���C���[�ƍ��G�͈͂��Ԃ��������H
	bool mIsWallHit;			// �ǂƂԂ����Ă��邩�H
	bool mIsLost;				// �v���C���[�������������H
	Actor* wallObj;				// �ǃI�u�W�F�N�g
	Actor* playerObj;			// �v���C���[�I�u�W�F�N�g
};