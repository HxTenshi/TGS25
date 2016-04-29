
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

	XMVECTOR mWindVector;
private:
	//�����o�ϐ�
	float mRotateY;
	bool isGround;     //�n�ʂƐڒn���Ă��邩�̃`�F�b�N
	bool isJump;       //�v���C���[���W�����v�������̃`�F�b�N

	float xRotate;
	float yRotate;

	float mYRot;
	float mXRot;

private:
	XMVECTOR RotationBoard();
	XMVECTOR Trick();
	void Jump();
};