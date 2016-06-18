
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
	//�����o�ϐ�
	SERIALIZE
	int mCredit; //�v���C���[�̎c�@
	SERIALIZE
	int mMaxPoint; //�������A�C�e���̐�
	SERIALIZE
	float mFadeOutSecond;

	Actor* mFadeOutObj;
};