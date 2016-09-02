
#pragma once
#include "main.h"
#include"h_standard.h"
#include"h_component.h"


enum BirdPosition
{
	LEFT,
	RIGHT
};

class CCSail :public IDllScriptComponent {
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	BirdPosition GetBirdPosition();

	float MovePower();
	float GetSailRotateRad();

private:
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
	float Lerp(float value1, float value2, float amount);
	void SailRotate();
	void SailAnimation();
};