#include "SquidInk.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SquidInk::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SquidInk::Start(){
	mDestroyTime *= 60;
}

//���t���[���Ă΂�܂�
void SquidInk::Update(){
	// �O���Ɉړ�
	auto forwardVelocity = gameObject->mTransform->Forward() * -mSpeed * 0.01f;

	auto position = gameObject->mTransform->Position();
	gameObject->mTransform->Position(position + forwardVelocity);


	mDestroyTime--;
	if (mDestroyTime <= 0) {
		game->DestroyObject(gameObject);
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void SquidInk::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SquidInk::OnCollideBegin(Actor* target){

	if (target->Name() == "Board") {

	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SquidInk::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void SquidInk::OnCollideExit(Actor* target){
	(void)target;
}