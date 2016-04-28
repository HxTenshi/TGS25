
#pragma once
#include "main.h"


class Enemy :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void PlayerSearch();
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;

protected:
	//ƒƒ“ƒo•Ï”
	float mSpeed;
	float mSize;
	bool mIsHit;
	bool mIsSearchRange;
};