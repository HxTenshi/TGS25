
#pragma once
#include "main.h"


class CameraController :public IDllScriptComponent{
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
	Actor* mTarget;
	XMVECTOR mPosition;

private:
	void Look();
};