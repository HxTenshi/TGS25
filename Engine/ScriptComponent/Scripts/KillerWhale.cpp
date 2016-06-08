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
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void KillerWhale::Start(){
	Enemy::Start();
	Enemy::SetDamage(mSetDamage);
	Enemy::SetResPawnTime(mSetResPawnTime);
	Enemy::AddPlayerChaseStopDistance(mAddChaseStopDistance);

	mInitBulletShotTime = mBulletShotTime;
	mInitRecastTime = mRecastTime;
	mGRAVITY = mParentObj->mTransform->Up() * 0.098f;
}

//���t���[���Ă΂�܂�
void KillerWhale::Update(){
	Enemy::Move();
	//game->Debug()->Log(std::to_string(mIsFloorHit));
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
}

void KillerWhale::ShortDistanceAttack() {

	mIsAttckMode = true;
	mAnimationID = 1;

	// ���ˎ��Ԃ�0�ɂȂ�܂Ńv���C���[�̕���������
	mBulletShotTime--;
	//game->Debug()->Log("����");
	if (mBulletShotTime <= 0) {
		if (!mIsShot) {
			// ���S�C�̒e�̐���
			auto gunBullet = game->CreateActor("Assets/Enemy/WaterGunBullet");
			game->AddObject(gunBullet);
			// �ʒu�̕ύX
			Enemy::SetParentForwardObj(gunBullet);

			mIsShot = true;
		}
		else {
			mRecastTime--;
			if (mRecastTime <= 0) {
				mBulletShotTime = mInitBulletShotTime;
				mRecastTime = mInitRecastTime;
				mIsShot = false;
				mIsAttckMode = false;
			}
		}	
	}
	else {
		Enemy::PlayerChaseMode();
	}

}

void KillerWhale::CenterDistanceAttack() {
	ShortDistanceAttack();
}

void KillerWhale::LongDistanceAttack() {
	Enemy::PlayerChaseMode();

	mAnimationID = 1;
	auto parentPosition = mParentObj->mTransform->Position();
	auto forwardMove = mParentObj->mTransform->Forward() * mSpeed * 0.01f;
	if (!mIsFloorHit) {
		forwardMove += mGRAVITY;
	}
	mParentObj->mTransform->Position(parentPosition - forwardMove);
}