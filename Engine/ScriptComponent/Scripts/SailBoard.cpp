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
#include"Sail.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SailBoard::Initialize(){

	mWindVector = XMVectorSet(1,0,0,1);
	isGround = false;
	isJump = false;
	isDead = false;
	mRotateX = 0;
	mYRot = 0.0f;
	mXRot = 0.0f;
	count = 0;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SailBoard::Start(){
	mPrevAcceler = Input::Analog(PAD_DS4_Velo3Coord::Velo3_Acceleration).y;
}

//���t���[���Ă΂�܂�
void SailBoard::Update(){

	isDead = Dead();
	ReSpawn();

	auto physx = gameObject->GetComponent<PhysXComponent>();
	if (physx) {
		auto v = physx->GetForceVelocity();
		v *= -0.5f;
		gameObject->mTransform->AddForce(v);

		auto mat = gameObject->GetComponent<MaterialComponent>();
		if(XMVector3Length(physx->GetForceVelocity()).x > 50)
		{
			if (mat) mat->SetAlbedoColor(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		if(!physx)
		{
			if (mat) mat->SetAlbedoColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	auto rotatey = RotationBoard();
	auto rotatex = Trick();

	if ((Input::Trigger(KeyCoord::Key_SPACE) || Shake()) && isGround)
	{
		Jump();
	}

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

		float power = 2.0f;
		auto v = XMVectorSet(0, 1, 0, 1);
		gameObject->mTransform->AddForce(v*power);
	}

	if (target->Name() == "PointItem"){

		count++;
		game->Debug()->Log(std::to_string(count));
		game->DestroyObject(target);
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SailBoard::OnCollideEnter(Actor* target){
	(void)target;

	if (target->Name() == "Air")
	{
		isGround = true;

		//�g�̕\���̃v���O����
		/*float power = 1.0f;
		auto v = XMVectorSet(0, 1, 0, 1);
		gameObject->mTransform->AddForce(v*power);*/
	}

	if (target->Name() == "Wind")
	{
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

	auto sail = game->FindActor("Sail");
	if (sail)
	{
		auto movepower = sail->GetScript<Sail>()->MovePower();
		mYRot += max(min(mRotateY, 5), -5) * 0.01f * movepower;
	}
	if (!isJump)
	{
		mYRot += max(min(mRotateY, 5), -5) * 0.05f;
	}
	
	if (Input::Trigger(PAD_DS4_KeyCoord::Button_CROSS))mRotateY = 0;
	auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), mYRot);
	return rotatey;
}

//�W�����v���ł���ΑO���]���\�ɂȂ�
XMVECTOR SailBoard::Trick()
{
	if (isJump)
	{
		if (Input::Down(KeyCoord::Key_W)) {
			mRotateX -= 0.05f;
		}
		if (Input::Down(KeyCoord::Key_S)) {
			mRotateX += 0.05f;
		}
		mRotateX -= Input::Analog(PAD_DS4_Velo3Coord::Velo3_Angular).y;
	}
	else //�W�����v���łȂ����X�̉�]�͏����ɖ߂�
	{
		mXRot = 0;
		mRotateX = 0;
	}

	mXRot += max(min(mRotateX, 5), -5) * 0.1f;

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

bool SailBoard::Dead()
{
	//if (gameObject->mTransform->Position().y < -7) return true;
	auto deadline = game->FindActor("DeadLine");
	if (deadline)
	{
		return deadline->mTransform->Position().y > gameObject->mTransform->Position().y;
	}
	return false;
}

void SailBoard::ReSpawn()
{
	if (isDead)
	{
		auto point = game->FindActor("ReSpawnPoint");
		if (point)
		{
			gameObject->mTransform->Position(point->mTransform->Position());
		}
	}
}

bool SailBoard::Shake()
{
	auto f = abs(mPrevAcceler - Input::Analog(PAD_DS4_Velo3Coord::Velo3_Acceleration).z);
	if (f > 0.5f)
	{
		return true;
	}
	mPrevAcceler = Input::Analog(PAD_DS4_Velo3Coord::Velo3_Acceleration).z;
	return false;
}
