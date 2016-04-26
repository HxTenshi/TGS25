
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

private:
	//メンバ変数
	float mRotateY;
	XMVECTOR mWindvec;

private:
	float MovePower();

};