
#pragma once
#include "main.h"
#include <vector>

class PlayerSearch;
class EnemyCG;
class MoveSmoke;
class EnemyManager;

enum class EnemyState {
	PlayerSearch,
	PlayerChase,
	PlayerShortDistance,
	PlayerCenterDistance,
	PlayerLongDistance,
	TornadoEscape,
	DeadMove,
	ReturnMove
};

enum class EnemyDeadState {
	KnockBackDead,
	BlowAwayDead,
	TornadoDead
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
	// ���G�ړ����܂�
	virtual void PlayerSearchMove();
	// ���G�֐��ł�
	virtual void PlayerSearchMode(const XMVECTOR objScale);
	// �v���C���[�̕����ɉ�]���܂�
	virtual void PlayerChaseMode(const float startPoint, const float endPoint);
	// �v���C���[��ǐՂ��܂�
	virtual void PlayerChaseMove();
	// �v���C���[�Ƃ̋������Z���Ƃ��̍s���ł�
	virtual void ShortDistanceAttack();
	// �v���C���[�Ƃ̋��������Ԃ̍s���ł�
	virtual void CenterDistanceAttack();
	// �v���C���[�Ƃ̋����������Ƃ��̍s���ł�
	virtual void LongDistanceAttack();
	// �������瓦����Ƃ��̍s���ł�
	virtual void TornadoEscapeMove(Actor* tornadoObj);
	// �v���C���[�ɓ���������m�b�N�o�b�N���܂�
	virtual void KnockBack();
	// �_���[�W�̐ݒ�ł�
	void SetDamage(int damage);
	// ���X�|�[���^�C���ł�
	void SetResPawnTime(int time);
	// ���G�͈͂̃T�C�Y��ύX���܂�
	void SetSearchRangeScale(const float scaleX, const float scaleY, const float scaleZ);
	// �ړ����̃X���b�N�I�u�W�F�𐶐����܂�
	void EnemyMoveSmoke();
	// ��苗���܂ŗ������烊�X�|�[�����܂�
	void ResPawnLine();
	// ������T���܂�
	void SearchTornado();
	// �G�̎��S�s���ł�
	void DeadMove();
	// ������ю��̎��S�s���ł�
	void DeadBlowAwayMove();
	// �����Ɋ������܂ꂽ���̎��S�s���ł�
	void DeadTornadoMove();
	// �����ɐ�����΂���鎞�̎��S�s���ł�
	void DeadTornadoBlowAwayMove();
	// �G�������I�u�W�F�N�g�ɖ߂�s���ł�
	void ReturnMove();
	// �v���C���[�{�[�g�̑ϋv�͉񕜏������s���܂�
	void PlayerHeal();
	// ���S�������s���܂�
	void Dead();
	// �G�̍s���֐�
	void Move();
	// �G�̏�Ԃ̕ύX���s���܂�
	void ChangeEnemyState(EnemyState state);
	// �G�̎��S��Ԃ̕ύX���s���܂�
	void ChangeEnemyDeadState(EnemyDeadState state);
	// �v���C���[�̒ǐՂ𒆎~���鋗���̉��Z�ł�
	void AddPlayerChaseStopDistance(float distance);
	// �����̃X�e�[�^�X�����܂�
	void SetTornadoStatus(
		const float power, const float rotate, const float addRotate,
		const float rotatePower, const float upPower, const float interval,
		const float distance);
	// �q��������A�j���[�V������ID��ς��܂�
	void SetAnimationID(int id);
	// �q��������A�j���[�V�����̃^�C����ύX���܂�(���g�p)
	void SetAnimationTime(float time);
	// �q��������A�j���[�V�����̃^�C���X�P�[����ύX���܂�
	void SetAnimationTimeScale(float timeScale);
	// �q��������A�j���[�V�����̃��[�v��ύX���܂�
	void SetAnimationLoop(bool isLoop);
	// ���݂̃A�j���[�V�����̃^�C�����擾���܂�
	float GetAnimationTime();
	// �f���^�^�C�����擾���܂�
	float GetEnemyDeltaTime(float framerate);

protected:
	//�����o�ϐ�
	Actor* mPlayerSearchObj;				// ���G�͈̓I�u�W�F�N�g
	Actor* mParentObj;						// ��̐e�I�u�W�F�N�g
	Actor* mEnemyCGObj;						// �G�̃O���t�B�b�N�I�u�W�F�N�g
	Actor* mTornadoObj;						// �����̃I�u�W�F�N�g
	Actor* mBlowAwayTornadoObj;				// ������ю��S���̗����̃I�u�W�F�N�g
	Actor* mRightSmokeObj;					// �E�̃X���b�N�I�u�W�F�N�g
	Actor* mLeftSmokeObj;					// ���̃X���b�N�I�u�W�F�N�g
	Actor* mEnemyManagerObj;				// �G�l�~�[�}�l�[�W���[�I�u�W�F�N�g
	Actor* mCreateEnemyObj;
	PlayerSearch* mSearchScript;			// ���G�͈̓I�u�W�F�N�g�̃X�N���v�g
	EnemyCG* mEnemyCGScript;				// �G��CG�I�u�W�F�N�g�̃X�N���v�g
	MoveSmoke* mRightSmokeScript;			// �E�̃X���b�N�I�u�W�F�N�g�̃X�N���v�g
	MoveSmoke* mLeftSmokeScript;			// ���̃X���b�N�I�u�W�F�N�g�̃X�N���v�g
	EnemyManager* mEnemyManagerScript;		// �G�l�~�[�}�l�[�W���[�̃X�N���v�g
	EnemyState mEnemyState;					// enum�N���X��EnemyState(�G�̍s���I�����Ɏg�p)
	EnemyDeadState mEnemyDeadState;			// enum�N���X��
	XMVECTOR mSize;							// �G�̑傫��
	XMVECTOR mSearchObjPosition;			// ���G�͈͂̈ʒu
	XMVECTOR mKnockBackDIrection;			// �m�b�N�o�b�N�̕���
	int mAddSearchObjCount;					// ���G�͈̓I�u�W�F�̍쐬�J�E���g
	int mDamage;							// �v���C���[�ւ̃_���[�W
	int mResPawnTime;						// ���X�|�[���^�C��
	int mInitResPawnTime;					// �������X�|�[���^�C��
	int mCGCreateCount;						// CG�̐����J�E���g(�S���̃��\�[�X�����������������)
	int mAnimationID;						// �A�j���[�V������ID
	int mInitSetCount;						// �ŏ��ɐݒ肵����
	int mTornadosCount;						// �����̐�
	float mSpeed;							// �G�̑��x�ύX
	float mHalfSizeZ;						// �G��Z�̑傫���̔���
	float mPositionY;						// �G��Y�̈ʒu�i���Ƃ̈ʒu�␳�Ɏg�p�j
	float mRotateY;							// �G�̉�]�p
	float mScalarX;							// �X�P�[���l(X)
	float mScalarY;							// �X�P�[���l(Y)
	float mScalarZ;							// �X�P�[���l(Z)
	float mAddPlayerChaseStopDistance;		// �ǐՒ��~�̋����ɉ��Z����l
	float mTornadoPower;					// �����̋z�����ޗ�
	float mTornadoRotateScale;				// �����ɉ�]����傫��
	float mAddTornadoRotateScale;			// �����ւ̉�]�̉��Z
	float mTornadoRotatePower;				// �����̉�]��
	float mTornadoUpPower;					// �����ɂ��㏸��
	float mTornadoInterval;					// �����̉�]���̊Ԋu�ł�
	float mTornadoDistance;					// �����ɔ������鋗��
	float mTornadoMinDistance;				// �ŒZ�̗������������܂�
	float mBlowAwayY;						// �����ɐ�����񂾂Ƃ���Y�̊p�x
	float mBlowAwayPower;					// ������ԑ��x
	float mBlowAwayInterval;				// ������ъԊu
	bool mIsMove;							// �����Ă��邩
	bool mIsFloorHit;						// ���Ɠ���������
	bool mIsCloudHit;						// �_�Ɠ���������
	bool mIsImmortalBody;					// �s���g�̓G��(������)
	bool mIsDistanceAct;					// ��������̍s�������邩
	bool mIsKnockBackDirection;				// �m�b�N�o�b�N�̕��������߂���
	bool mIsChaseRotate;					// �v���C���[�̕������������ǂ���
	bool mIsAttckMode;						// �U���r����(�����O�ɏo���ꍇ�̍U�����~�̖h�~)
	bool mIsTornadoBlowAway;				// �����Ő�����΂���
	bool mIsPlayerHeal;						// �v���C���[���񕜂�����
	bool mIsCreateChase;					// �������Ƀv���C���[��ǂ���
	// �����s���R���e�i
	typedef std::vector<EnemyState> DistanceVector;
	DistanceVector mDistanceVector;

private:
	const float mResPawnHeigth = -100.0f;	// ���X�|�[�����鍂��
};