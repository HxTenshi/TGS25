
#pragma once
#include "main.h"


class PlayerController :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;

private:
	//�����o�ϐ�

	XMVECTOR mJump;
	XMVECTOR mGravity;
	XMVECTOR mToQuaternion;
};