
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
	// �G�̐F�̐ݒ�ł�(�f�o�b�O�p)
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
	// �v���C���[�ɓ���������m�b�N�o�b�N���܂�
	virtual void KnockBack();
	// �G�̖ڂ̑O�ɃI�u�W�F�𐶐����܂�(�e������ꍇ)
	void SetParentForwardObj(Actor* setObj);
	// �_���[�W�̐ݒ�ł�
	void SetDamage(int damage);
	// ���X�|�[���^�C���ł�
	void SetResPawnTime(int time);
	// ��苗���܂ŗ������烊�X�|�[�����܂�
	void ResPawnLine();
	// �G�̍s���֐�
	void Move();
	// �G�̃X�e�[�^�X�����������܂�
	void InitStatus();
	// �G�̃X�e�[�^�X�����Z�b�g���܂�
	void ResetStatus();
	// �v���C���[�̒ǐՂ𒆎~���鋗���̉��Z�ł�
	void AddPlayerChaseStopDistance(float distance);
	// �v���C���[�Ǝw�肳�ꂽ�I�u�W�F�̈ʒu�Ƃ̋������v�Z���ĕԂ��܂�
	float GetPlayerDistance(Actor* playerObj, Actor* otherObj);
	// �A�j���[�V������ID��n���܂�
	int GetAnimationID();

protected:
	//�����o�ϐ�
	Actor* mPlayerSearchObj;				// ���G�͈̓I�u�W�F�N�g
	Actor* mParentObj;						// ��̐e�I�u�W�F�N�g
	Actor* mEnemyCGObj;						// �G�̃O���t�B�b�N�I�u�W�F�N�g
	PlayerSearch* mSearchScript;			// ���G�͈̓I�u�W�F�N�g�̃X�N���v�g
	EnemyState mEnemyState;					// enum�N���X��EnemyState(�G�̍s���I�����Ɏg�p)
	XMVECTOR mSize;							// �G�̑傫��
	XMVECTOR mInitPosition;					// �����ʒu
	XMVECTOR mInitRotate;;					// �����̉�]
	XMVECTOR mSearchObjPosition;			// ���G�͈͂̈ʒu
	XMVECTOR mKnockBackHoukou;				// �m�b�N�o�b�N�̕���
	int mAddSearchObjCount;					// ���G�͈̓I�u�W�F�̍쐬�J�E���g
	int mDamage;							// �v���C���[�ւ̃_���[�W
	int mResPawnTime;						// ���X�|�[���^�C��
	int mInitResPawnTime;					// �������X�|�[���^�C��
	int mCGCreateCount;						// CG�̐����J�E���g(�S���̃��\�[�X�����������������)
	int mAnimationID;						// �A�j���[�V������ID
	int mInitSetCount;
	float mSpeed;							// �G�̑��x�ύX
	float mHalfSizeZ;						// �G��Z�̑傫���̔���
	float mPositionY;						// �G��Y�̈ʒu�i���Ƃ̈ʒu�␳�Ɏg�p�j
	float mRotateY;							// �G�̉�]�p
	float mScalarX;							// �X�P�[���l(X)
	float mScalarY;							// �X�P�[���l(Y)
	float mScalarZ;							// �X�P�[���l(Z)
	float mAddPlayerChaseStopDistance;		// �ǐՒ��~�̋����ɉ��Z����l
	//float mInitParentPositionY;				// �e�̏����ʒu(Y)
	const float mResPawnHeigth = -10.0f;	// ���X�|�[�����鍂��
	bool mIsFloorHit;						// ���Ɠ���������
	bool mIsImmortalBody;					// �s���g�̓G��(������)
	bool mIsDistanceAct;					// ��������̍s�������邩
	bool mIsAttckMode;						// �U���r����(�����O�ɏo���ꍇ�̍U�����~�̖h�~)
	bool mIsDead;							// �v���C���[�Ɠ���������
	// �s���z��
	typedef std::vector<EnemyState> DistanceVector;
	DistanceVector mDistanceVector;
};