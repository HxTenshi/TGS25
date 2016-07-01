
#pragma once
#include "main.h"


class EnemyManager :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// �G�̖ڂ̑O�ɃI�u�W�F�𐶐����܂�(�e������ꍇ)
	void SetParentForwardObj(Actor* setObj);
	// �e�̃X�e�[�^�X�̏����������܂�
	void InitStatus();
	// �G�̃X�e�[�^�X�̃��Z�b�g���܂�
	void ResetStatus();
	// �T�E���h���Đ����܂�
	void EnemyPlaySound(const std::string soundName);
	// �v���C���[�Ǝw�肳�ꂽ�I�u�W�F�̈ʒu�Ƃ̋������v�Z���ĕԂ��܂�
	float GetPlayerDistance(Actor* playerObj, Actor* otherObj);
	// �f���^�^�C�����擾���܂�
	float GetEnemyDeltaTime(float framerate);

private:
	//�����o�ϐ�
	XMVECTOR mInitParentPosition;					// �e�̏����ʒu
	XMVECTOR mInitParentRotate;;					// �e�̏����̌���
	Actor* mEnemyObj;
	Actor* mEnemyParentObj;
};