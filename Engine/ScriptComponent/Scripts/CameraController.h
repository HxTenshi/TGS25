
#pragma once
#include "main.h"


class CameraController :public IDllScriptComponent {
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	void StateUpdate(float deltaTime);
	void Standby(float deltaTIme);
	void Normal(float deltaTime);
	void Jump(float deltaTime);
	void Dead(float deltaTime);
	void Tornado(float deltaTime);


private:
	//メンバ変数
	Actor* mTarget;
	XMVECTOR mPosition;

	bool mIsDead;
	XMVECTOR mNextPos;
	float mTimer;
	float mAngle;

private:
	void Look();
	XMVECTOR Lerp(XMVECTOR p1, XMVECTOR p2);
};