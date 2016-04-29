
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

private:
	XMVECTOR RotationBoard();
	XMVECTOR Trick();
	void Jump();

private:
	//メンバ変数
	bool isGround;     //地面と接地しているかのチェック
	bool isJump;       //プレイヤーがジャンプしたかのチェック

	float mRotateX;
	float mRotateY;

	float mYRot;
	float mXRot;

	XMVECTOR mWindVector;


};