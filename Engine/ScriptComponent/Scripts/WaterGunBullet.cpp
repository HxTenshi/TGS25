#include "WaterGunBullet.h"
#include "SailBoard.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WaterGunBullet::Initialize(){
	
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WaterGunBullet::Start(){
	mDestroyTime *= 60;
}

//���t���[���Ă΂�܂�
void WaterGunBullet::Update(){
	// �O���Ɉړ�
	mForwardVelocity = gameObject->mTransform->Forward() * -mSpeed * 0.01f;

	auto position = gameObject->mTransform->Position();
	gameObject->mTransform->Position(position + mForwardVelocity);

	mSpeed -= 0.01f;

	mDestroyTime--;
	if (mDestroyTime <= 0) {
		game->DestroyObject(gameObject);
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void WaterGunBullet::Finish(){
	//game->DestroyObject(gameObject);
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WaterGunBullet::OnCollideBegin(Actor* target){

	if (target->Name() == "Board") {
		auto playerScript = target->GetComponent<SailBoard>();
		playerScript->Damage(mSetDamege);

		game->DestroyObject(gameObject);
		target->mTransform->AddForce(gameObject->mTransform->Forward() * -100.0f);
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WaterGunBullet::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void WaterGunBullet::OnCollideExit(Actor* target){
	(void)target;
}