#include "SailBoard.h"

#include"Wind.h"
#include "Input/Input.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"PhysX\IPhysXEngine.h"

#include"Game\Component\MaterialComponent.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SailBoard::Initialize(){

	mWindVector = XMVectorSet(1,0,0,1);
	isGround = false;
	isJump = false;
	xRotate = 0;
	yRotate = 0;
	mYRot = 0.0f;
	mXRot = 0.0f;
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

	auto rotatey = RotationBoard();
	auto rotatex = Trick();

	if ((Input::Trigger(KeyCoord::Key_SPACE) || Input::Analog(PAD_DS4_Velo3Coord::Velo3_Acceleration).z > 0) && isGround)
	{
		Jump();
	}

	/*float power = 4.0f;
	auto v = XMVectorSet(0, 1, 0, 1);
	gameObject->mTransform->AddForce(v*power);*/

	gameObject->mTransform->Quaternion(XMQuaternionMultiply(rotatex,rotatey));
	

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void SailBoard::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SailBoard::OnCollideBegin(Actor* target){
	(void)target;

	if (target->Name() == "Air"){
		isJump = false;
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

	if (target->Name() == "Air")
	{
		isGround = true;
		/*float power = 1.0f;
		auto v = XMVectorSet(0, 1, 0, 1);
		gameObject->mTransform->AddForce(v*power);*/
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
		/*float power = 1.0f;
		auto v = XMVectorSet(0, 1, 0, 1);
		gameObject->mTransform->AddForce(v*power);*/
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

bool SailBoard::GetIsJump()
{
	return this->isJump;
}

//�{�[�h�̍��E��]
XMVECTOR SailBoard::RotationBoard()
{
	if (Input::Down(KeyCoord::Key_A)) {
		mRotateY -= 0.01f;
	}
	if (Input::Down(KeyCoord::Key_D)) {
		mRotateY += 0.01f;
	}
	mRotateY += Input::Analog(PAD_DS4_Velo3Coord::Velo3_Angular).x;
	mYRot += max(min(mRotateY, 5), -5) * 0.01f;

	auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), mYRot);
	return rotatey;
}

//�W�����v���ł���ΑO���]���\�ɂȂ�
XMVECTOR SailBoard::Trick()
{
	if (isJump)
	{
		if (Input::Down(KeyCoord::Key_W)) {
			xRotate -= 0.05f;
		}
		if (Input::Down(KeyCoord::Key_S)) {
			xRotate += 0.05f;
		}
		xRotate -= Input::Analog(PAD_DS4_Velo3Coord::Velo3_Angular).y;
	}
	if(!isJump)
	{
		mXRot = 0;
		xRotate = 0;
	}

	mXRot += max(min(xRotate, 5), -5) * 0.1f;

	auto rotatez = XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 1), mXRot);
	return rotatez;
}

void SailBoard::Jump()
{
	isJump = true;
	float power = 300.0f;
	auto v = XMVectorSet(0, 1, 0, 1);
	gameObject->mTransform->AddForce(v*power);
}
