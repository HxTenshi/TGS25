
#pragma once
#include "main.h"


class PlayerSearch :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	bool IsPlayerSearch();

private:
	//メンバ変数
	float mSizeZ;
	bool mIsPlayerHit;
};