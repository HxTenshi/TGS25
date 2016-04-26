#include "Sail.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"
#include "Engine\DebugEngine.h"

#include"Wind.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Sail::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Sail::Start(){
	mRotateY = 0.00f;
}

//���t���[���Ă΂�܂�
void Sail::Update(){

	if (Input::Down(KeyCoord::Key_A)) {
		mRotateY -= 0.05f;
	}
	if (Input::Down(KeyCoord::Key_D)) {
		mRotateY += 0.05f;
	}
	
	auto parent = gameObject->mTransform->GetParent();
	parent->mTransform->AddForce(parent->mTransform->Forward() * MovePower() * 5);

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Sail::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Sail::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Sail::OnCollideEnter(Actor* target){

}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Sail::OnCollideExit(Actor* target){
	(void)target;
}

//���̌����{�Z�C���Ő��i�͂̌v�Z
float Sail::MovePower()
{
	auto wind = game->FindActor("Wind");
	auto windscr = wind->GetScript<Wind>();
	mWindvec = windscr->GetWindVelocity();
	mWindvec.y = 0;

	auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), mRotateY);
	gameObject->mTransform->Rotate(rotatey);

	auto temprotate = gameObject->mTransform->Left();

	temprotate = XMVector3Normalize(temprotate);
	mWindvec = XMVector3Normalize(mWindvec);

	auto rad = XMVector3Dot(mWindvec, temprotate);

	return abs(rad.x);
}
