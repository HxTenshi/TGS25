
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

private:
	//�����o�ϐ�
	SERIALIZE
	int mCredit; //�v���C���[�̎c�@
};