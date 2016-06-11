
#pragma once
#include "main.h"


class PointItem :public IDllScriptComponent{
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
	float mAngel;
	bool isPlayerFind;
	SERIALIZE
		float FindDistance;
	SERIALIZE
		float Speed;
};