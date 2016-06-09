
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
	//プレイヤーは無敵か?
	bool IsUnrivaled();
	//ダメージの処理
	void Damage(int damage);
	//トリックできたか？
	bool IsTrick();
	//プレイヤーの体力の入手
	float GetHitPoint();

private:
	void RotationBoard();
	void Trick();
	void Jump();
	bool Dead();
	void ReSpawn();
	bool Shake();
	void AnimationChange(int id,bool loop);
	

private:
	//メンバ変数
	bool isGround;     //地面と接地しているかのチェック
	bool isJump;       //プレイヤーがジャンプしたかのチェック
	bool isDead;

	//プレイヤーのトリックが成功したか
	bool mTrick;

	//地上用の変数
	float mRotateY;
	float mYRot;
	//トリック中の変数
	XMVECTOR mTrickRotate;
	float mTrickPoint; //トリックの点数


	float mJumpYRotate; //ジャンプ中の回転
	float mPrevAcceler;
	XMVECTOR mWindVector;

	//プレイヤーのHP
	float mPlyerHP;

	Actor* mBird;

	SERIALIZE
	float AttackSpeed; //攻撃できるスピード
	SERIALIZE
	float SlipDamege; //受けるダメージ
	SERIALIZE
	float JumpPower; //ジャンプの力
	SERIALIZE
	float RecoveryPoint;

	//エフェクト関連
	std::string bombName;
	Actor* mSpeedEffect;
	//--------------------------------------

};