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
	
	mSpeed = 4.0f * 0.01f;
	mMotionCount = 0;
	mIsShot = false;
	//// �s���g����
	//mIsImmortalBody = true;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void KillerWhale::Start(){
	Enemy::Start();
}

//���t���[���Ă΂�܂�
void KillerWhale::Update(){
	//Enemy::PlayerColorChange(gameObject);
	Enemy::PlayerSearchMode(gameObject);
	Enemy::PlayerChaseMode(gameObject);
	//Enemy::PlayerChase(gameObject);
	Enemy::FallDead(gameObject);
	//game->Debug()->Log(std::to_string(Enemy::PlayerDistance()));

	//game->Debug()->Log(std::to_string(mIsSearchRange));
	auto PlayerObj = game->FindActor("Board");

	mMotionCount++;
	if (mMotionCount >= 60 * 3) {
		if (!mIsShot) {
			// ���S�C�̒e�̐���
			auto GunBullet = game->CreateActor("Assets/WaterGunBullet");
			game->AddObject(GunBullet);
			// �ʒu�̕ύX
			Enemy::SetForwardObj(GunBullet);
		}
		mIsShot = true;
	}

	//auto thisPosi = gameObject->mTransform->Position();
	//thisPosi = XMVector3Normalize(thisPosi);
	//auto otherPosi = PlayerObj->mTransform->Position();
	//otherPosi = XMVector3Normalize(otherPosi);
	////auto radius = XMVector3AngleBetweenVectors(thisPosi, otherPosi);
	//auto radius = XMVector3AngleBetweenNormals(otherPosi , thisPosi);
	////auto angle = XMConvertToDegrees(radius);
	//auto radiusZ = radius.z * 180 / (float)XM_PI;

	//game->Debug()->Log(std::to_string(radiusZ));
	
	// ���vector �ōs���̔z������A size������]����o����if�����Ȃ�������
	if (mIsSearchRange) {
		if (Enemy::PlayerDistance() <= mSize.z * 1.0f + mHalfSizeZ ||
			Enemy::PlayerDistance() <= mSize.z * 2.0f + mHalfSizeZ) {
			mMotionCount++;
			//game->Debug()->Log("����");
			if (mMotionCount >= 60 * 3) {
				if (!mIsShot) {
					// ���S�C�̒e�̐���
					auto GunBullet = game->CreateActor("Assets/WaterGunBullet");
					game->AddObject(GunBullet);
					// �ʒu�̕ύX
					Enemy::SetForwardObj(GunBullet);
				}
				mIsShot = true;
			}
			else if (mMotionCount >= 60 * 5) {
				Enemy::PlayerChase(gameObject);
			}
		}
		else if (Enemy::PlayerDistance() > mSize.z * 2.0f + mHalfSizeZ ||
			Enemy::PlayerDistance() <= mSize.z * 3.0f + mHalfSizeZ) {
			//game->Debug()->Log("�܂�");
			Enemy::PlayerChase(gameObject);
		}
	}

	auto objPosition = gameObject->mTransform->Position();
	auto forwardMove = gameObject->mTransform->Forward() * mSpeed;
	// Y���̕␳ (�G�̈ꕔ���C�ʂɏo�Ă���悤�ɂ���)
	if (mIsFloorHit) {	
		auto setPosition = XMVectorSet(objPosition.x, mPositionY, objPosition.z, 0.0f);
		gameObject->mTransform->Position(setPosition + forwardMove);
	}
	else {
		gameObject->mTransform->Position(objPosition + forwardMove);
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void KillerWhale::Finish(){
	//Enemy::Finish();
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void KillerWhale::OnCollideBegin(Actor* target){
	(void)target;
	Enemy::OnCollideBegin(target);
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void KillerWhale::OnCollideEnter(Actor* target){
	(void)target;
	Enemy::OnCollideEnter(target);
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void KillerWhale::OnCollideExit(Actor* target){
	(void)target;
	Enemy::OnCollideExit(target);
}