#include "Squid.h"
#include "Enemy.h"
#include "PlayerSearch.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"
#include "Input/Input.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Squid::Initialize(){
	Enemy::Initialize();

	mInitMoveInterval = mMoveInterval;
	mInitWaitInterval = mWaitInterval;

	mSetDamage = 0;
	mSpeed = mSetSpeed;
	mIsPlayerHit = false;
	mSRPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Squid::Start(){
	Enemy::Start();
	Enemy::SetDamage(mSetDamage);
	Enemy::SetResPawnTime(mSetResPawnTime);
	Enemy::AddPlayerChaseStopDistance(mAddChaseStopDistance);
}

//���t���[���Ă΂�܂�
void Squid::Update(){
	Enemy::Move();
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Squid::Finish(){
	Enemy::Finish();
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Squid::OnCollideBegin(Actor* target){
	Enemy::OnCollideBegin(target);
	
	if (target->Name() == "Board") {
		mIsPlayerHit = true;
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Squid::OnCollideEnter(Actor* target){
	Enemy::OnCollideEnter(target);
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Squid::OnCollideExit(Actor* target){
	Enemy::OnCollideExit(target);
}

void Squid::SearchMove() {

	// �e�𓮂���
	auto parentPosition = mParentObj->mTransform->Position();
	auto forwardMove = mParentObj->mTransform->Forward() * (mSpeed * 0.01f);

	if (mMoveInterval > 0) {
		mParentObj->mTransform->Position(parentPosition - forwardMove);
		mMoveInterval--;
	}
	else {
		if (mWaitInterval > 0) {
			mWaitInterval--;
		}
		else {
			mMoveInterval = mInitMoveInterval;
			mWaitInterval = mInitWaitInterval;
		}
	}
}

void Squid::PlayerChase() {	

	// �v���C���[�̕����������i�܂��j
	auto playerObj = game->FindActor("Board");
	auto playPo = playerObj->mTransform->Position();
	// �e�̃X�e�[�^�X�̎擾
	auto parPo = mParentObj->mTransform->Position();
	auto parRo = mParentObj->mTransform->Rotate();

	// �v���C���[�Ɠ���������A�v���C���[�Ɋ����t��
	if (mIsPlayerHit) {
		// twineAround
		auto playerScale = playerObj->mTransform->Scale();
		auto twineAroundPosition = XMVectorSet(playPo.x, playPo.y + (playerScale.y / 2.0f), playPo.z, 0.0f);
		mParentObj->mTransform->Position(twineAroundPosition);
		// ����Ɏx����������X�N���v�g���Ă�
	}
}