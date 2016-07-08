
#pragma once
#include "main.h"
#include <vector>

enum class EnemyName {
	FlyingFishName = 0,
	KillerWhaleName = 1,
	RandomName = 2
};

class CreateEnemyObj :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// �����I�u�W�F�N�g�̖��O��ݒ肵�܂�
	void CreateName();
	// �����_���֐��ł�
	int GetRandom(int min, int max);
	// �G�������I�u�W�F�N�g�ɖ߂鋗����Ԃ��܂�
	float GetReturnDistance();

private:
	//�����o�ϐ�
	Actor* mCreateObj;				// �����I�u�W�F�N�g
	int mVectorCount;
	SERIALIZE
	int mCreateCount;				// �����J�E���g
	SERIALIZE
	int mOnceCreateCount;			// ���ɐ�������G�̐�
	SERIALIZE
	int mCreateTimeInterval;		// �G�����̊Ԋu
	int mInitCreateTimeInterval;	// �����̐����Ԋu
	SERIALIZE
	int mCreateDelayTime;			// �G�������̃f�B���C�^�C��
	int mInitCreateDelayTime;		// �����̃f�B���C�^�C��
	SERIALIZE
	int mRandomCreateRange;			// �����_���������̔z�u�Ԋu
	//�������Ɏg�p
	SERIALIZE
	float mReactionDistance;		// ��������
	SERIALIZE
	float mEnemyReturnDistance;		// �G�������I�u�W�F�N�g�ɖ߂鋗��
	bool mIsCreateObj;				// �G�𐶐��������H
	SERIALIZE
	bool mIsRandom;					// �������g�p���邩�H
	bool mIsInfinityCreate;			// ���������ɂ��邩�H
	const char* mCreateObjName;		// �����I�u�W�F�N�g�̖��O
	SERIALIZE
	std::string mEnemyName;			// ��������G�̖��O
	std::string mBaseStr;			// ���O�̌�
	// �����I�u�W�F�N�g�̖��O�R���e�i
	typedef std::vector<std::string> NameContainer;
	NameContainer mNameContainer;
};