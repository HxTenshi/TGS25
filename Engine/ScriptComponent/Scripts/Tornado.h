
#pragma once
#include "main.h"


class Tornado :public IDllScriptComponent{
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
	SERIALIZE
		float x;
	SERIALIZE
		float y;
	SERIALIZE
		float LimitTime;

	float mTimer;

};