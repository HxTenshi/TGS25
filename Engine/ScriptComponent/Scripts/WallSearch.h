
#pragma once
#include "main.h"


class WallSearch :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// 壁にぶつかったかを返します
	bool IsWallHit();

private:
	// ぶつかったオブジェクトが目的のオブジェクトならtureを返します
	bool IsNameHit(Actor* target);

private:
	//メンバ変数
	bool mIsWallHit;
};