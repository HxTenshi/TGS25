#include "WaterGunBullet.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WaterGunBullet::Initialize(){
	mSpeed = 4.0f * 0.01f;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WaterGunBullet::Start(){

}

//���t���[���Ă΂�܂�
void WaterGunBullet::Update(){
	mForwardVelocity = gameObject->mTransform->Forward() * mSpeed;
	auto position = gameObject->mTransform->Position();
	gameObject->mTransform->Position(position + mForwardVelocity);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void WaterGunBullet::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WaterGunBullet::OnCollideBegin(Actor* target){
	(void)target;
	if (target->Name() == "Board") {
		game->DestroyObject(gameObject);
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