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

//生成時に呼ばれます（エディター中も呼ばれます）
void SailBoard::Initialize(){

	mWindVector = XMVectorSet(1,0,0,1);
	isGround = false;
	isJump = false;
	mRotateX = 0;
	mYRot = 0.0f;
	mXRot = 0.0f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void SailBoard::Start(){
}

//毎フレーム呼ばれます
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

	gameObject->mTransform->Quaternion(XMQuaternionMultiply(rotatex,rotatey));
	

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void SailBoard::Finish(){

}

//コライダーとのヒット時に呼ばれます
void SailBoard::OnCollideBegin(Actor* target){
	(void)target;

	if (target->Name() == "Air"){
		isJump = false;

		float power = 2.0f;
		auto v = XMVectorSet(0, 1, 0, 1);
		gameObject->mTransform->AddForce(v*power);
	}

	if (target->Name() == "PointItem"){

		game->DestroyObject(target);
	}
}

//コライダーとのヒット中に呼ばれます
void SailBoard::OnCollideEnter(Actor* target){
	(void)target;

	if (target->Name() == "Air")
	{
		isGround = true;

		//波の表現のプログラム
		/*float power = 1.0f;
		auto v = XMVectorSet(0, 1, 0, 1);
		gameObject->mTransform->AddForce(v*power);*/
	}

	if (target->Name() == "Wind")
	{
		if(target->GetScript<Wind>()) mWindVector = target->GetScript<Wind>()->GetWindVelocity();
	}
}

//コライダーとのロスト時に呼ばれます
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

//ボードの左右回転
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
	if (!isGround)
	{
		mYRot += max(min(mRotateY, 5), -5) * 0.05f;
	}
	
	if (Input::Trigger(PAD_DS4_KeyCoord::Button_PS))mRotateY = 0;
	auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), mYRot);
	return rotatey;
}

//ジャンプ中であれば前後回転が可能になる
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

	//ジャンプ中でなければXの回転は初期に戻す
	if(!isJump)
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
