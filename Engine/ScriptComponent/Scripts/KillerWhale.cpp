#include "KillerWhale.h"
#include "Enemy.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void KillerWhale::Initialize(){
	Enemy::Initialize();
	
	mSpeed = mSetSpeed;
	mIsShot = false;
	mIsDistanceAct = true;
	mIsBlowAway = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void KillerWhale::Start(){
	Enemy::Start();
	Enemy::SetDamage(mSetDamage);
	Enemy::SetResPawnTime(mSetResPawnTime);
	Enemy::AddPlayerChaseStopDistance(mAddChaseStopDistance);
	Enemy::SetSearchRangeScale(mSetSearchRengeScaleX, mSetSearchRengeScaleY, mSetSearchRengeScaleZ);
	Enemy::SetTornadoStatus(mSetTornadoPower, mSetTornadoBlowAwayInterval, mSetTornadoDistance);

	mInitBulletShotTime = mBulletShotTime;
	mInitRecastTime = mRecastTime;
	mGRAVITY = mParentObj->mTransform->Up() * 0.098f;
}

//���t���[���Ă΂�܂�
void KillerWhale::Update(){
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

void KillerWhale::SearchMove() {
	// �������Ȃ�
	//game->Debug()->Log(std::to_string(mIsFloorHit));
	if (!mIsFloorHit && mInitSetCount == 1) {
		auto parentPosition = mParentObj->mTransform->Position();
		mParentObj->mTransform->Position(parentPosition - mGRAVITY);
	}
	Enemy::SetAnimationID(0);
	Enemy::SetAnimationTimeScale(1.0f * mSpeed * 0.2f);
	Enemy::SetAnimationLoop(true);
}

void KillerWhale::ShortDistanceAttack() {

	mIsAttckMode = true;
	//mAnimationID = 1;
	Enemy::SetAnimationID(1);
	//Enemy::SetAnimationLoop(false);

	//game->Debug()->Log("����");
	if (mBulletShotTime <= 0) {
		if (!mIsShot) {
			// ���S�C�̒e�̐���
			auto gunBullet = game->CreateActor("Assets/Enemy/WaterGunBullet");
			game->AddObject(gunBullet);
			// �ʒu�̕ύX
			Enemy::SetParentForwardObj(gunBullet);

			Enemy::SetAnimationTimeScale(1.0f);

			mIsShot = true;
			Enemy::SetAnimationLoop(false);
		}
		else {
			mRecastTime--;
			if (mRecastTime <= 0) {
				mBulletShotTime = mInitBulletShotTime;
				mRecastTime = mInitRecastTime;
				mIsShot = false;
				mIsAttckMode = false;
				Enemy::SetAnimationLoop(true);
			}
		}	
	}
	else {
		// ���ˎ��Ԃ�0�ɂȂ�܂Ńv���C���[�̕���������
		mBulletShotTime--;
		Enemy::PlayerChaseMode(0.0f, 0.0f);
		Enemy::SetAnimationTimeScale(2.0f * (35.0f / mInitBulletShotTime));
	}

}

void KillerWhale::CenterDistanceAttack() {
	ShortDistanceAttack();
}

void KillerWhale::LongDistanceAttack() {
	Enemy::PlayerChaseMode(0.0f, 0.0f);

	Enemy::SetAnimationID(0);
	Enemy::SetAnimationLoop(true);

	auto parentPosition = mParentObj->mTransform->Position();
	auto forwardMove = mParentObj->mTransform->Forward() * mSpeed * 0.01f;
	if (!mIsFloorHit) {
		forwardMove += mGRAVITY;
	}
	mParentObj->mTransform->Position(parentPosition - forwardMove);
}