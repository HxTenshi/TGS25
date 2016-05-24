
#pragma once
#include "main.h"

class PlayerSearch;


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
	virtual void PlayerColorChange(Actor* obj);
	virtual void SearchMove();
	virtual void PlayerSearchMode(Actor* thisObj, const float sizeX, const float sizeY, const float sizeZ);
	virtual void PlayerSearchMode(Actor* thisObj); // エラー防止用
	virtual void PlayerChaseMode(Actor* thisObj);
	virtual void PlayerChase(Actor* thisObj);
	virtual void PlayerSpaceChase(Actor* thisObj, const int interval, int count);
	virtual void SetForwardObj(Actor* setObj);
	virtual void SetForwardChildrenObj(Actor* setObj);
	virtual void FallDead(Actor* thisObj);
	virtual float PlayerDistance();

protected:
	//メンバ変数
	Actor* mPlayerSearchObj;	// 索敵範囲オブジェクト
	PlayerSearch* searchScript;	// 索敵範囲オブジェクトのスクリプト
	XMVECTOR mSize;
	int mAddSearchObjCount;
	float mSpeed;
	float mHalfSizeZ;
	float mPositionY;
	float mRotateY;
	float mDeadHeigth;
	bool mIsObjHit;
	bool mIsFloorHit;
	bool mIsSearchRange;
	bool mIsImmortalBody;
};