#include "SailBoard.h"

#include"Wind.h"
#include "Input/Input.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SailBoard::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SailBoard::Start(){

}

//���t���[���Ă΂�܂�
void SailBoard::Update(){

	auto physx = gameObject->GetComponent<PhysXComponent>();
	if (physx) {
		auto v = physx->GetForceVelocity();
		v *= -0.5f;
		gameObject->mTransform->AddForce(v);
	}

	RotationBoard();

	float power = 4.0f;
	auto v = XMVectorSet(0, 1, 0, 1);
	gameObject->mTransform->AddForce(v*power);

	

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void SailBoard::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SailBoard::OnCollideBegin(Actor* target){
	(void)target;

	/*if (target->Name() == "Air"){

		auto pos = gameObject->mTransform->Position();
		
		float power = 2.0f;
		auto v = XMVectorSet(0, 1, 0, 1);
		gameObject->mTransform->AddForce(v*power);
	}*/

	if (target->Name() == "PointItem"){

		game->DestroyObject(target);
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SailBoard::OnCollideEnter(Actor* target){
	(void)target;

	if (target->Name() == "Air"){
		auto pos = gameObject->mTransform->Position();

		float power = 2.0f;
		auto v = XMVectorSet(0, 1, 0, 1);
		gameObject->mTransform->AddForce(v*power);
	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void SailBoard::OnCollideExit(Actor* target){
	(void)target;
}

void SailBoard::RotationBoard()
{
	if (Input::Down(KeyCoord::Key_O)) {
		mRotateY -= 0.05f;
	}
	if (Input::Down(KeyCoord::Key_P)) {
		mRotateY += 0.05f;
	}

	auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), mRotateY);
	gameObject->mTransform->Rotate(rotatey);
}
