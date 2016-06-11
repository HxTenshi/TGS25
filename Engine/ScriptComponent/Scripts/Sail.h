
#pragma once
#include "main.h"


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
	//ƒƒ“ƒo•Ï”
	float mSailRotate;
	XMVECTOR mVelocity;
	XMVECTOR mWindvec;
	SERIALIZE
	float Speed;

private:
	void SailRotate();

};