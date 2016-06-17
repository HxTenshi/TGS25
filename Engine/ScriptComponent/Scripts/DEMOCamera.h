
#pragma once
#include "main.h"


class DEMOCamera :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;

private:
	//ÉÅÉìÉoïœêî
	Actor* mTarget;
	XMVECTOR mPosition;

	bool mPrevJump;

	XMVECTOR mFromPos;
	float mTimer;

	void Look();
	XMVECTOR Lerp(XMVECTOR p1, XMVECTOR p2);

	SERIALIZE
	float offsetx;
	SERIALIZE
	float offsety;
	SERIALIZE
	float offsetz;

	SERIALIZE
	float rotatex;
	SERIALIZE
	float rotatey;
	SERIALIZE
	float rotatez;

	XMVECTOR offset;
	XMVECTOR rotate;
};