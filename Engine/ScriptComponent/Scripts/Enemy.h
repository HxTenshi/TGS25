
#pragma once
#include "main.h"
#include <vector>

class PlayerSearch;

enum class EnemyState {
	PlayerSearch,
	PlayerChase,
	PlayerShortDistance,
	PlayerCenterDistance,
	PlayerLongDistance
};

//typedef std::vector<EnemyState> distanceVector;

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
	// �G�̐F�̐ݒ�ł�
	virtual void PlayerColorChange();
	// ���G�ړ����܂�
	virtual void SearchMove();
	// ���G�֐��ł�
	virtual void PlayerSearchMode(const XMVECTOR objScale);
	// �v���C���[�̕����ɉ�]���܂�
	virtual void PlayerChaseMode();
	// �v���C���[��ǐՂ��܂�
	virtual void PlayerChase();
	// �v���C���[�Ƃ̋������Z���Ƃ��̍s���ł�
	virtual void ShortDistanceAttack();
	// �v���C���[�Ƃ̋��������Ԃ̍s���ł�
	virtual void CenterDistanceAttack();
	// �v���C���[�Ƃ̋����������Ƃ��̍s���ł�
	virtual void LongDistanceAttack();
	// �G�̖ڂ̑O�ɃI�u�W�F�𐶐����܂�
	virtual void SetForwardObj(Actor* setObj);
	// �_���[�W�̐ݒ�ł�
	virtual void setDamage(int damage);
	// ��苗���܂ŗ������玀�S���܂�
	virtual void FallDead(Actor* thisObj);
	// �v���C���[�Ƃ̋����̌v�Z���܂� (Z���W)
	virtual float PlayerDistance();
	// �G�̍s���֐�
	void Move();

protected:
	//�����o�ϐ�
	Actor* mPlayerSearchObj;			// ���G�͈̓I�u�W�F�N�g
	Actor* mParentObj;					// ��̐e�I�u�W�F�N�g
	PlayerSearch* searchScript;			// ���G�͈̓I�u�W�F�N�g�̃X�N���v�g
	EnemyState mEnemyState;				// enum�N���X��EnemyState(�G�̍s���I�����Ɏg�p)
	XMVECTOR mSize;						// �G�̑傫��
	XMVECTOR mSearchObjPosition;		// ���G�͈͂̈ʒu
	int mAddSearchObjCount;				// ���G�͈̓I�u�W�F�̍쐬�J�E���g
	int mDamage;						// �v���C���[�ւ̃_���[�W
	float mSpeed;						// �G�̑��x�ύX
	float mHalfSizeZ;					// �G��Z�̑傫���̔���
	float mPositionY;					// �G��Y�̈ʒu�i���Ƃ̈ʒu�␳�Ɏg�p�j
	float mRotateY;						// �G�̉�]�p	
	float mScalarX;
	float mScalarY;
	float mScalarZ;
	const float mDeadHeigth = -10.0f;	// ���S���鍂��
	bool mIsFloorHit;					// ���Ɠ���������
	bool mIsImmortalBody;				// �s���g�̓G��(������)
	bool mIsDistanceAct;				// ��������̍s�������邩
	bool mIsAttckMode;					// �U���r����(�����O�ɏo���ꍇ�̍U�����~�̖h�~)
};