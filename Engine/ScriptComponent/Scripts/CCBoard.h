
#pragma once
#include "main.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"
#include"CCSail.h"

enum State
{
	STANDBY,       //開始する前
	MOVE,          //地面に接地してるとき
	JUMP,          //ジャンプしたとき
	TORNADO,       //トルネードを出したとき
	DEAD           //死亡したとき 
};

enum SailPos
{
	LEFTPOS,
	RIGHTPOS
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
	XMVECTOR GetWind();
	int GetState();
	void Damage(int damage);
	bool IsTrick();
	float GetHitPoint();
	bool IsUnrivaled();

private:
	void StateUpdate(float deltaTime);
	void Standby(float deltaTime);
	void Move(float deltaTime);
	void Jump(float deltaTime);
	void Tornado(float deltaTime);
	void Dead(float deltaTime);
	void StateChange(State next);
	void GroundCheck();
	void Trick(float deltaTime);
	void CreateTornado();
	void CreateAttackWind();
	void MoveSmokeParameterSet(float speed, float max = 0);
	bool Shake();

	void PlaySE(std::string filename);


private: //アニメーション関連 クラスに分けたいけどウィークポインタのクラス間の渡し方がわからない
	void AnimationChange(int id);
	void SailRotateAnimation();


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
		float mSlipDamage;
	SERIALIZE
		float mTrickCondition;
	SERIALIZE
		float mTrickRotatePower;

	XMVECTOR mVelocity;
	float mCurrentSpeed;
	bool isJump;
	bool isTornado;

	XMVECTOR mTrickRotate;
	float mTrickPoint; //トリックの点数
	Actor* mTornado;
	Actor* mSpeedEffect;
	Actor* mArrow;

	float mPrevAcceler;
	XMVECTOR mWindVector;
	float mPlyerHP;
	float mPoint;

};