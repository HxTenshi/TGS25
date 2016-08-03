
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
	void Normal(float deltaTime);
	void Jump(float deltaTime);
	void Dead(float deltaTime);
	void Tornado(float deltaTime);


private:
	//ÉÅÉìÉoïœêî
	Actor* mTarget;
	XMVECTOR mPosition;

	bool mPrevJump;

	XMVECTOR mFromPos;
	float mTimer;

private:
	void Look();
	XMVECTOR Lerp(XMVECTOR p1, XMVECTOR p2);
};