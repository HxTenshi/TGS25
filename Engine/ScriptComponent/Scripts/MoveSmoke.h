
#pragma once
#include "main.h"


class MoveSmoke :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	void SetMaxSpeed(float max);
	void SetSpeed(float speed);

private:
	//メンバ変数
	SERIALIZE
	float mMaxSpeed;
	SERIALIZE
	float mSpeed;
};