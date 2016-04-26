
#pragma once
#include "main.h"


class SailBoard :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	XMVECTOR GetWind();

	XMVECTOR mWindVector;
private:
	//ƒƒ“ƒo•Ï”
	float mRotateY;

private:
	void RotationBoard();
};