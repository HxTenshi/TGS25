
#pragma once
#include "main.h"
#include "Game/Actor.h"
#include"Game\Component\AnimationComponent.h"

class Actor;


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
	bool GetIsGround();
	bool IsDead();
	//�v���C���[�͖��G��?
	bool IsUnrivaled();
	//�_���[�W�̏���
	void Damage(int damage);
	//�g���b�N�ł������H
	bool IsTrick();
	//�v���C���[�̗̑͂̓���
	float GetHitPoint();

private:
	void RotationBoard();
	void Trick();
	void Jump();
	bool Dead();
	void ReSpawn();
	bool Shake();
	void PlaySE(std::string filename);
	void ArrowUpdate();


	void AnimationChange(int id,bool loop,float timer);
	void AnimationChange(int id, bool loop, float timer,float scale);
	void AnimationReversePlay(int id, bool loop);
	void AnimationReverse(float TimeScale = 1.0f);
	bool IsCurrentAnimation();
	

private:
	//�����o�ϐ�
	int mPoint;

	bool isGround;     //�n�ʂƐڒn���Ă��邩�̃`�F�b�N
	bool isJump;       //�v���C���[���W�����v�������̃`�F�b�N
	bool isDead;

	//�v���C���[�̃g���b�N������������
	bool mTrick;

	//�n��p�̕ϐ�
	float mRotateY;
	float mYRot;
	//�g���b�N���̕ϐ�
	XMVECTOR mTrickRotate;
	float mTrickPoint; //�g���b�N�̓_��


	float mJumpYRotate; //�W�����v���̉�]
	float mPrevAcceler;
	XMVECTOR mWindVector;
	Actor* mArrow;

	//�v���C���[��HP
	float mPlyerHP;
	weak_ptr<AnimationComponent> mAnimator;

	SERIALIZE
	float AttackSpeed; //�U���ł���X�s�[�h
	SERIALIZE
	float SlipDamege; //�󂯂�_���[�W
	SERIALIZE
	float JumpPower; //�W�����v�̗�
	SERIALIZE
	float RecoveryPoint;

	//�G�t�F�N�g�֘A
	std::string bombName;
	Actor* mSpeedEffect;
	//--------------------------------------

};