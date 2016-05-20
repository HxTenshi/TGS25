
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
	//プレイヤーは無敵か?
	bool IsUnrivaled();
	//ダメージの処理
	void Damage(int damage);

private:
	XMVECTOR RotationBoard();
	XMVECTOR Trick();
	void Jump();
	bool Dead();
	void ReSpawn();
	bool Shake();
	

private:
	//メンバ変数
	bool isGround;     //地面と接地しているかのチェック
	bool isJump;       //プレイヤーがジャンプしたかのチェック
	bool isDead;

	//プレイヤーが無敵かどうか
	bool mUnrivaled;

	float mRotateX;
	float mRotateY;

	float mYRot;
	float mXRot;

	float mPrevAcceler;
	XMVECTOR mWindVector;

	//プレイヤーのHP
	float mPlyerHP;


};