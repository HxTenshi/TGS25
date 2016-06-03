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
	mIsAttckMode = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void KillerWhale::Start(){
	Enemy::Start();
	Enemy::SetDamage(mSetDamage);
	Enemy::SetResPawnTime(mSetResPawnTime);
	//EnemyCGCreate("Shachi/shachi", 0.1f, 0.1f, 0.1f);

	/*mBulletShotTime *= 60;
	mRecastTime *= 60;*/
	mInitBulletShotTime = mBulletShotTime;
	mInitRecastTime = mRecastTime;
}

//���t���[���Ă΂�܂�
void KillerWhale::Update(){
	/*if (mParentCreateCount == 1) {

		if (mCGCreateCount == 0) {
			Enemy::EnemyCGCreate();
			mCGCreateCount = 1;
		}
	}*/

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

}

void KillerWhale::ShortDistanceAttack() {

	mBulletShotTime--;
	//game->Debug()->Log("����");
	if (mBulletShotTime <= 0) {
		if (!mIsShot) {
			// ���S�C�̒e�̐���
			auto GunBullet = game->CreateActor("Assets/Enemy/WaterGunBullet");
			game->AddObject(GunBullet);
			// �ʒu�̕ύX
			Enemy::SetForwardObj(GunBullet);
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

	if (!mIsAttckMode) {
		auto parentPosition = mParentObj->mTransform->Position();
		auto forwardMove = mParentObj->mTransform->Forward() * mSpeed * 0.01f;
		// Y���̕␳ (�G�̈ꕔ���C�ʂɏo�Ă���悤�ɂ���)
		if (mIsFloorHit) {
			/*auto setPosition = XMVectorSet(objPosition.x, mPositionY, objPosition.z, 0.0f);
			gameObject->mTransform->Position(setPosition + forwardMove);*/
		}
		else {
			mParentObj->mTransform->Position(parentPosition - forwardMove);
		}
	}
}