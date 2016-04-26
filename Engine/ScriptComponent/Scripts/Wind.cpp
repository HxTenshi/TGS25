#include "Wind.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Wind::Initialize(){

	mWindVelocity = XMVectorSet(1, 0, 0, 1);

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Wind::Start(){

}

//���t���[���Ă΂�܂�
void Wind::Update()
{
	/*auto obj = game->FindActor("Board");
	if (obj)
	{
		obj->mTransform->AddForce(mWindVelocity);
	}*/
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Wind::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Wind::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Wind::OnCollideEnter(Actor* target){
	/*if (target->Name() == "Board")
	{
		target->mTransform->AddForce(mWindVelocity);
	}*/
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Wind::OnCollideExit(Actor* target){
	(void)target;
}

XMVECTOR Wind::GetWindVelocity(){
	return mWindVelocity;
}