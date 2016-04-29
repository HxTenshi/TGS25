
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
	//メンバ変数
	float mRotateY;
	bool isGround;     //地面と接地しているかのチェック
	bool isJump;       //プレイヤーがジャンプしたかのチェック

	float xRotate;
	float yRotate;

	float mYRot;
	float mXRot;

private:
	XMVECTOR RotationBoard();
	XMVECTOR Trick();
	void Jump();
};