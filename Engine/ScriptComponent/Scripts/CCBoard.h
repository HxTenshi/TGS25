
#pragma once
#include "main.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"

enum State
{
	MOVE,          //�n�ʂɐڒn���Ă�Ƃ�
	JUMP,          //�W�����v�����Ƃ�
	TORNADO,       //�g���l�[�h���o�����Ƃ�
	DEAD           //���S�����Ƃ� 
};

class CCBoard :public IDllScriptComponent {
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	int GetState();
	void Damage(int damage);
	bool IsTrick();
	float GetHitPoint();
	bool IsUnrivaled();

private:
	void StateUpdate(float deltaTime);
	void Move(float deltaTime);
	void Jump(float deltaTime);
	void Tornado(float deltaTime);
	void Dead(float deltaTime);
	void StateChange(State next);
	void GroundCheck();
	void Trick();
	void CreateTornado();
	void CreateAttackWind();
	void MoveSmokeParameterSet(float speed, float max = 0);


private: //�A�j���[�V�����֘A �N���X�ɕ����������ǃE�B�[�N�|�C���^�̃N���X�Ԃ̓n�������킩��Ȃ�
	void AnimationChange(int id);


private:
	State mState;

	SERIALIZE
		float mSpeed;
	SERIALIZE
		float mMaxSpeed;
	SERIALIZE
		float mRotateSpeed;
	SERIALIZE
		float mJumpPower;
	SERIALIZE
		float mAttackSpeed;

	SERIALIZE
		std::string mCameraName;

	XMVECTOR mVelocity;
	float mCurrentSpeed;
	bool isJump;
	bool isTornado;

	XMVECTOR mTrickRotate;
	float mTrickPoint; //�g���b�N�̓_��
	Actor* mTornado;
	Actor* mSpeedEffect;

	float mPlyerHP;

};