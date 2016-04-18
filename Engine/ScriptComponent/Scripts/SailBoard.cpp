#include "SailBoard.h"

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

	float power = 1.0f;
	if (Input::Down(KeyCoord::Key_W)){


		auto v = gameObject->mTransform->Forward();
		gameObject->mTransform->AddForce(v*power);
	}
	if (Input::Down(KeyCoord::Key_S)){

		auto v = gameObject->mTransform->Forward();
		gameObject->mTransform->AddForce(v*-power);
	}


	power = 15.0f;
	if (Input::Down(KeyCoord::Key_A)){
		auto v = gameObject->mTransform->Up();
		gameObject->mTransform->AddTorque(v*-power);
	}
	if (Input::Down(KeyCoord::Key_D)){
		auto v = gameObject->mTransform->Up();
		gameObject->mTransform->AddTorque(v*power);
	}
	if (Input::Down(KeyCoord::Key_Z)){
		auto v = gameObject->mTransform->Up();
		gameObject->mTransform->AddTorque(v*power*100);
	}

	power = 8.0f;
	auto v = XMVectorSet(0, 1, 0, 1);
	gameObject->mTransform->AddForce(v*power);

	auto physx = gameObject->GetComponent<PhysXComponent>();
	if (physx){
		auto v = physx->GetForceVelocity();
		v *= -0.5f;
		gameObject->mTransform->AddForce(v);
	}

	//auto rot = gameObject->mTransform->Rotate();
	//rot.y = 0;
	////rot.z = 0;
	//gameObject->mTransform->Rotate(rot);

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void SailBoard::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SailBoard::OnCollideBegin(Actor* target){
	(void)target;

	if (target->Name() == "Air"){

		auto pos = gameObject->mTransform->Position();
		
		float power = 2.0f;
		auto v = XMVectorSet(0, 1, 0, 1);
		gameObject->mTransform->AddForce(v*power);
	}

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
