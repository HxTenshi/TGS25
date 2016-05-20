
#pragma once
#include "main.h"


class SailBoard :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	XMVECTOR GetWind();
	bool GetIsJump();
	//�v���C���[�͖��G��?
	bool IsUnrivaled();
	//�_���[�W�̏���
	void Damage(int damage);
	//�g���b�N�ł������H
	bool IsTrick();
	//�v���C���[�̗̑͂̓���
	float GetHitPoint();

private:
	XMVECTOR RotationBoard();
	XMVECTOR Trick();
	void Jump();
	bool Dead();
	void ReSpawn();
	bool Shake();
	

private:
	//�����o�ϐ�
	bool isGround;     //�n�ʂƐڒn���Ă��邩�̃`�F�b�N
	bool isJump;       //�v���C���[���W�����v�������̃`�F�b�N
	bool isDead;

	//�v���C���[�̃g���b�N������������
	bool mTrick;

	float mRotateX;
	float mRotateY;

	float mYRot;
	float mXRot;

	float mJumpYRotate; //�W�����v���̉�]
	float mPrevAcceler;
	XMVECTOR mWindVector;

	//�v���C���[��HP
	float mPlyerHP;


};