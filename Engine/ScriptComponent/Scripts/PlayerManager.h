
#pragma once
#include "main.h"


class PlayerManager :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;

	void CreditDown();
	void CreditUp();
	void ItemGet();

private:
	//メンバ変数
	SERIALIZE
	int mCredit; //プレイヤーの残機
	SERIALIZE
	int mMaxPoint; //回収するアイテムの数
};