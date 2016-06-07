
#pragma once
#include "main.h"


class EnemyCG :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	void ChangeAnimation(int id);

private:
	//ƒƒ“ƒo•Ï”
	int mAnimationID;
	int mInitAnimationID;
};