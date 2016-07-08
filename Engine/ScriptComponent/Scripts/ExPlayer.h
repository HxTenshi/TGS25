
#pragma once
#include "main.h"


class ExPlayer :public IDllScriptComponent{
public:
	ExPlayer();
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;

private:
	//ÉÅÉìÉoïœêî
	SERIALIZE
	float mSpeed;
	SERIALIZE
	float mMaxSpeed;
	SERIALIZE
	float mRotateSpeed;
	SERIALIZE
	float mJumpPower;

	SERIALIZE
	std::string mCameraName;

	float mCurrentSpeed;
	int mGroundHitCount;
};