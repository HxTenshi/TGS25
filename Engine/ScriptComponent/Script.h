
#pragma once
#include "main.h"

class PlayerScript :public IDllScriptComponent{
public:
	PlayerScript();
	void Update()override;
	void OnCollide(Actor* target) override;
	float mRotateY;

};
class CameraScript :public IDllScriptComponent{
public:
	void Start()override;
	void Update()override;

	void GameClear();

private:
	float mRotateY;
	float mLength;
	bool mClear;
};

class CoinScript : public IDllScriptComponent{
public:
	void Start()override;
	void Update()override;
private:
	float mRotateY;
};

class CoinManagerScript : public IDllScriptComponent{
public:
	void Start()override;
	void Update()override;
	void GetCoin();
private:
	int mGetCoinCount;
	int mMaxCoin;
	bool isClear;
};

class Cursor :public IDllScriptComponent{
public:
	Cursor();
	void Update()override;

};
class Block;
class BlockManager :public IDllScriptComponent{
public:
	void Initialize()override;
	void SetBlock(Block* block);
	void MoveBlock(XMVECTOR& pos,XMVECTOR& move);
private:
	int x;
	int y;
	Block* mBlockMap[10][10];

};
class Block :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Move(XMVECTOR& move);
	bool IsMovement();
private:
	bool mIsMovement;
	int mMoveTimer;
	XMVECTOR mMoveParam;
};
class GoalPocket :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void OnCollide(Actor* target) override;
private:
	int mWaterMax;
	int mWaterGoalCount;
};