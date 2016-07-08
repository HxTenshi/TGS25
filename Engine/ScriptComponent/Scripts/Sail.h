
#pragma once
#include "main.h"

enum BirdPosition
{
	LEFT,
	RIGHT
};

class Sail :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	float MovePower();
	float GetSailRotateRad();


private:
	//メンバ変数
	float move;
	float mSailRotate;
	XMVECTOR mVelocity;
	XMVECTOR mWindvec;
	SERIALIZE
	float Speed;
	float mRotate;
	BirdPosition mBirdPos;
	bool isAnimation;

	SERIALIZE
	float RotatePower;

private:
	void SailRotate();
	void SailAnimation();

};