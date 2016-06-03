
#pragma once
#include "main.h"

class Enemy;

class ParentObj :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	void SetPosition(float x, float z);

private:
	//ƒƒ“ƒo•Ï”
	Enemy* mEnemyScript;
	//std::string mEnemyScriptName;
	int mCreateCount;
	float mPositionX;
	float mPositionZ;
};