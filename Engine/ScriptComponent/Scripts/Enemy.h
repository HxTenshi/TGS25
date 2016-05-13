
#pragma once
#include "main.h"


class Enemy :public IDllScriptComponent{
public:
	virtual ~Enemy();
	virtual void Initialize()override;
	virtual void Start()override;
	virtual void Update()override;
	virtual void Finish()override;
	virtual void OnCollideBegin(Actor* target)override;
	virtual void OnCollideEnter(Actor* target)override;
	virtual void OnCollideExit(Actor* target)override;
	//float norm(XMVECTOR a);
	//float dot(XMVECTOR a, XMVECTOR b);
	//float cross(XMVECTOR a, XMVECTOR b);
	//int CCW(XMVECTOR p0, XMVECTOR p1, XMVECTOR p2);
	//bool IsInSector(XMVECTOR c, float a, float b, float r, XMVECTOR p);
	virtual void PlayerColorChange(Actor* obj);
	virtual void SearchMove();
	virtual void PlayerSearchMode(Actor* thisObj);
	virtual void PlayerChaseMode(Actor* thisObj);
	virtual void PlayerChase(Actor* thisObj);
	virtual void PlayerSpaceChase(Actor* thisObj, const int interval, int count);
	virtual void FallDead(Actor* thisObj);

protected:
	//ÉÅÉìÉoïœêî
	float mSpeed;
	float mSize;
	float mPositionY;
	float mRotateY;
	float mDeadHeigth;
	bool mIsHit;
	bool mIsSearchRange;
};