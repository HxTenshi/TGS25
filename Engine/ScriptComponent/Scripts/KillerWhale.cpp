#include "KillerWhale.h"
#include "Enemy.h"
#include  "EnemyManager.h"

//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"
#include "Engine\DebugEngine.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void KillerWhale::Initialize(){
	Enemy::Initialize();
	
	mSpeed = mSetSpeed;
	mIsShot = false;
	mIsDistanceAct = true;
	//mIsBlowAway = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void KillerWhale::Start(){
	Enemy::Start();
	Enemy::SetDamage(mSetDamage);
	Enemy::SetResPawnTime(mSetResPawnTime);
	Enemy::AddPlayerChaseStopDistance(mAddChaseStopDistance);
	Enemy::SetSearchRangeScale(mSetSearchRengeScaleX, mSetSearchRengeScaleY, mSetSearchRengeScaleZ);
	Enemy::SetTornadoStatus(
		mSetTornadoPower, mSetTornadoRotateScale, mSetAddTornadoRotateScale,
		mSetTornadoRotatePower, mSetTornadoUpPower, mSetTornadoInterval,
		mSetTornadoDistance);
	Enemy::ChangeEnemyDeadState(EnemyDeadState::KnockBackDead);

	mInitBulletShotTime = mBulletShotTime;
	mInitRecastTime = mRecastTime;
	mGRAVITY = mParentObj->mTransform->Up() * 0.098f;
}

//���t���[���Ă΂�܂�
void KillerWhale::Update(){
	// ����y�����鏈��
	if (!mIsFloorHit) {
		auto parentPosition = mParentObj->mTransform->Position();
		mParentObj->mTransform->Position(parentPosition - mGRAVITY * Enemy::GetEnemyDeltaTime(60.0f));
	}
	Enemy::Move();
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void KillerWhale::Finish(){
	Enemy::Finish();
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void KillerWhale::OnCollideBegin(Actor* target){
	Enemy::OnCollideBegin(target);
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void KillerWhale::OnCollideEnter(Actor* target){
	Enemy::OnCollideEnter(target);
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void KillerWhale::OnCollideExit(Actor* target){
	Enemy::OnCollideExit(target);
}

void KillerWhale::PlayerSearchMove() {
	// �������Ȃ�
	mIsMove = false;

	Enemy::SetAnimationID(0);
	Enemy::SetAnimationTimeScale(1.0f * (mSpeed * 0.01f));
	Enemy::SetAnimationLoop(true);
}

void KillerWhale::ShortDistanceAttack() {
	mIsMove = false;
	// �U�����[�h���I���ɂ���
	mIsAttckMode = true;
	Enemy::SetAnimationID(1);
	// �J�E���g���O�ɂȂ����琅�S�C�̒e�𔭎�
	if (mBulletShotTime <= 0) {
		if (!mIsShot) {
			// ���S�C�̒e�̐���
			auto gunBullet = game->CreateActor("Assets/Enemy/WaterGunBullet");
			game->AddObject(gunBullet);
			// �ʒu�̕ύX
			mEnemyManagerScript->SetParentForwardObj(gunBullet);
			mIsShot = true;
			Enemy::SetAnimationTimeScale(1.0f);
			//// �T�E���h�̍Đ�
			//Enemy::EnemyPlaySound("tekitou");
		}
		else {
			mRecastTime -= Enemy::GetEnemyDeltaTime(30.0f);
			Enemy::SetAnimationTimeScale(2.0f * (15.0f / mInitRecastTime));
			if (mRecastTime <= 0) {
				mBulletShotTime = mInitBulletShotTime;
				mRecastTime = mInitRecastTime;
				mIsShot = false;
				mIsAttckMode = false;
				Enemy::SetAnimationTime(0.0f);
			}
		}	
	}
	else {
		// ���ˎ��Ԃ�0�ɂȂ�܂Ńv���C���[�̕���������
		mBulletShotTime -= Enemy::GetEnemyDeltaTime(30.0f);
		Enemy::PlayerChaseMode(0.0f, 0.0f);
		Enemy::SetAnimationTimeScale(2.0f * (34.0f / mInitBulletShotTime));
	}

	if (Enemy::GetAnimationTime() >= 40.0f) {
		Enemy::SetAnimationLoop(false);
	}
	else {
		Enemy::SetAnimationLoop(true);
	}
}

void KillerWhale::CenterDistanceAttack() {
	ShortDistanceAttack();
}

void KillerWhale::LongDistanceAttack() {
	mIsMove = true;
	Enemy::PlayerChaseMode(0.0f, 0.0f);

	Enemy::SetAnimationID(0);
	Enemy::SetAnimationTimeScale(1.0f * (mSpeed * 0.01f));
	Enemy::SetAnimationLoop(true);
	auto parentPosition = mParentObj->mTransform->Position();
	auto forwardMove = mParentObj->mTransform->Forward() * mSpeed * 0.01f;
	// ���ɓ������Ă��Ȃ��Ȃ痎����(���Ƃŏ���������)
	if (!mIsFloorHit) {
		forwardMove += mGRAVITY;
	}
	
	mParentObj->mTransform->Position(
		(parentPosition - forwardMove
		* Enemy::GetEnemyDeltaTime(60.0f)));
}