#include "SailBoard.h"

#include"Wind.h"
#include "Input/Input.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SailBoard::Initialize(){

	mWindVector = XMVectorSet(1,0,0,1);
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
	game->Debug()->Log("x=" + std::to_string(mWindVector.x));
	game->Debug()->Log("y=" + std::to_string(mWindVector.y));
	game->Debug()->Log("z=" + std::to_string(mWindVector.z));

	

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

	if (target->Name() == "Wind")
	{
		game->Debug()->Log("sng");
		//mWindVector = XMVectorSet(1, 0, 0, 1);
		if(target->GetScript<Wind>()) mWindVector = target->GetScript<Wind>()->GetWindVelocity();
		else game->Debug()->Log("ng");

	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void SailBoard::OnCollideExit(Actor* target){
	(void)target;
}

XMVECTOR SailBoard::GetWind()
{
	return this->mWindVector;
	//return XMVectorSet(1, 0, 0, 1);
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
