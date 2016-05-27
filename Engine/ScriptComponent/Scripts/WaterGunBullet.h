
#pragma once
#include "main.h"


class WaterGunBullet :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;

private:
	//メンバ変数
	SERIALIZE
	int mDestroyTime;
	SERIALIZE
	int mSetDamege;
	SERIALIZE
	float mSpeed;
	XMVECTOR mForwardVelocity;
};