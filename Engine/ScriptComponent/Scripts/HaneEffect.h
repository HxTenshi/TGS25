
#pragma once
#include "main.h"


class HaneEffect :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	void SetPosition(XMVECTOR pos);

private:
	//ƒƒ“ƒo•Ï”
	float mTimer;
	XMVECTOR mPosition;
	XMVECTOR mSetPosition;
	XMVECTOR mScale;
	bool isEnd;
};