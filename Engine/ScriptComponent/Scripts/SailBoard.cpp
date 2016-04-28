#include "SailBoard.h"

#include"Wind.h"
#include "Input/Input.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"

#include"Game\Component\MaterialComponent.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SailBoard::Initialize(){

	mWindVector = XMVectorSet(1,0,0,1);
	isGround = false;
	zRotate = 0;
	yRotate = 0;
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

	if (!isGround)
	{
		Trick();
	}

	if (Input::Trigger(KeyCoord::Key_SPACE) && isGround)
	{
		float power = 300.0f;
		auto v = XMVectorSet(0, 1, 0, 1);
		gameObject->mTransform->AddForce(v*power);
	}

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

	if (target->Name() == "Air")
	{
		isGround = true;
		float power = 1.0f;
		auto v = XMVectorSet(0, 1, 0, 1);
		gameObject->mTransform->AddForce(v*power);
	}

	if (target->Name() == "Wind")
	{
		//mWindVector = XMVectorSet(1, 0, 0, 1);
		if(target->GetScript<Wind>()) mWindVector = target->GetScript<Wind>()->GetWindVelocity();

	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void SailBoard::OnCollideExit(Actor* target){
	(void)target;

	if (target->Name() == "Air")
	{
		isGround = false;
	}

	if (target->Name() == "Wind")
	{
		mWindVector = XMVectorSet(1, 0, 0, 1);

	}
}

XMVECTOR SailBoard::GetWind()
{
	return this->mWindVector;
	//return XMVectorSet(1, 0, 0, 1);
}

void SailBoard::RotationBoard()
{
	if (Input::Down(KeyCoord::Key_A)) {
		mRotateY -= 0.05f;
	}
	if (Input::Down(KeyCoord::Key_D)) {
		mRotateY += 0.05f;
	}

	auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), mRotateY);
	gameObject->mTransform->Quaternion(rotatey);
}

void SailBoard::Trick()
{
	
	/*if (Input::Down(KeyCoord::Key_D)) {
		yRotate -= 0.05f;
	}
	if (Input::Down(KeyCoord::Key_A)) {
		yRotate += 0.05f;
	}*/

	if (Input::Down(KeyCoord::Key_W)) {
		zRotate -= 0.05f;
	}
	if (Input::Down(KeyCoord::Key_S)) {
		zRotate += 0.05f;
	}
	
	/*auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), yRotate);
	gameObject->mTransform->Rotate(rotatey);*/

	auto rotatez = XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 1), zRotate);
	gameObject->mTransform->Rotate(XMQuaternionMultiply(rotatez, gameObject->mTransform->Rotate()));
}
