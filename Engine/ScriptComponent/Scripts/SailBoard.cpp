#include "SailBoard.h"

#include"Wind.h"
#include "Input/Input.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include"Game\Component\AnimationComponent.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"PhysX\IPhysXEngine.h"

#include"Game\Component\MaterialComponent.h"
#include"Sail.h"
#include"PlayerManager.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void SailBoard::Initialize(){

	mWindVector = XMVectorSet(1,0,0,1);
	isGround = false;
	isJump = false;
	isDead = false;
	mYRot = 0.0f;
	mTrickPoint = 0;
	mPlyerHP = 100.0f;
	mJumpYRotate = 0;
	mTrickRotate = XMVectorSet(0, 0, 0, 0);
	mSpeedEffect = nullptr;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void SailBoard::Start(){
	mPrevAcceler = Input::Analog(PAD_DS4_Velo3Coord::Velo3_Acceleration).y;
	mBird = game->FindActor("Bird");
}

//毎フレーム呼ばれます
void SailBoard::Update(){
	
	if (Input::Down(KeyCoord::Key_Z)) mPlyerHP--;
	mPlyerHP -= SlipDamege;
	IsUnrivaled();
	isDead = Dead();
	ReSpawn();

	auto physx = gameObject->GetComponent<PhysXComponent>();
	if (physx) {
		auto v = physx->GetForceVelocity();
		v *= -0.5f;
		gameObject->mTransform->AddForce(v);
	}

	if (isJump)
	{
		Trick();
	}
	else
	{
		RotationBoard();
		mTrickPoint = 0;
		mTrickRotate = XMVectorSet(0, 0, 0, 0);
	}

	//ジャンプ用
	if ((Input::Trigger(KeyCoord::Key_SPACE) || Shake()) && isGround)
	{
		Jump();
	}


	//qｷｰとShareボタンで姿勢を戻す
	if (Input::Down(KeyCoord::Key_Q) || Input::Trigger(PAD_DS4_KeyCoord::Button_SHARE))
	{
		physx->SetForceVelocity(XMVectorSet(0, 0, 0, 1));
		gameObject->mTransform->Quaternion(XMQuaternionRotationAxis(gameObject->mTransform->Left(), 0));
	}
	
	mPlyerHP = min(max(mPlyerHP, -100), 100);

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void SailBoard::Finish(){

}

//コライダーとのヒット時に呼ばれます
void SailBoard::OnCollideBegin(Actor* target){
	(void)target;

	if (target->Name() == "Air" || target->Name() == "Floor"){
		isJump = false;

		if (!isGround)
		{
			auto bomb = game->CreateActor("Assets/tgs/SmokeBomb.json");
			if (bomb)
			{
				game->AddObject(bomb);
				bomb->mTransform->Position(gameObject->mTransform->Position());
			}
			isGround = true;
		}

		float power = 2.0f;
		auto v = XMVectorSet(0, 1, 0, 1);
		gameObject->mTransform->AddForce(v*power);
		gameObject->mTransform->Quaternion(XMQuaternionMultiply(XMQuaternionRotationAxis(gameObject->mTransform->Left(), 0),gameObject->mTransform->Quaternion()));
	}

	if (target->Name() == "PointItem"){

		auto manager = game->FindActor("PlayerManager")->GetScript<PlayerManager>();
		manager->ItemGet();
		game->DestroyObject(target);
	}
}

//コライダーとのヒット中に呼ばれます
void SailBoard::OnCollideEnter(Actor* target){
	(void)target;

	if (target->Name() == "Air" || target->Name() == "Floor")
	{
		if (IsTrick())
		{
			mPlyerHP += RecoveryPoint;
		}
		AnimationChange(0, false);
		mTrick = false;
		mJumpYRotate = 0;

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

	if (target->Name() == "Air" || target->Name() == "Floor")
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

bool SailBoard::IsUnrivaled()
{
	auto physx = gameObject->GetComponent<PhysXComponent>();
	if (XMVector3Length(physx->GetForceVelocity()).x > AttackSpeed)
	{
		auto temp = game->CreateActor("Assets/SpeedWind.json");
		if (temp)
		{
			if (!mSpeedEffect)
			{
				game->AddObject(temp);
				temp->mTransform->SetParent(gameObject);
				mSpeedEffect = temp;
			}
		}
		return true;
	}
	else
	{
		if (mSpeedEffect)
		{
			game->DestroyObject(mSpeedEffect);
			mSpeedEffect = nullptr;
		}
		return false;
	}
}

void SailBoard::Damage(int damage)
{
	mPlyerHP -= damage;
}

bool SailBoard::IsTrick()
{
	return mTrick;
}

float SailBoard::GetHitPoint()
{
	return mPlyerHP;
}

//ボードの左右回転(地上用)
void SailBoard::RotationBoard()
{
	if (Input::Down(KeyCoord::Key_A)) {
		mRotateY -= 0.1f;
	}
	if (Input::Down(KeyCoord::Key_D)) {
		mRotateY += 0.1f;
	}
	mRotateY += Input::Analog(PAD_DS4_Velo3Coord::Velo3_Angular).x;
	mYRot += mRotateY;
	auto sail = game->FindActor("Sail");
	if (sail)
	{
		auto movepower = sail->GetScript<Sail>()->MovePower();
		mYRot *= movepower * 0.1f;
		auto rotatey = XMQuaternionRotationAxis(gameObject->mTransform->Up(), mYRot);
		gameObject->mTransform->AddTorque(rotatey, ForceMode::eVELOCITY_CHANGE);

	}

	if (Input::Trigger(PAD_DS4_KeyCoord::Button_CROSS))
	{
		mRotateY = 0;
		mYRot = 0;
	}
}

//ジャンプ中であれば前後回転が可能になる
void SailBoard::Trick()
{
		if (Input::Down(KeyCoord::Key_W)) {
			mTrickRotate.x -= 0.05f;
		}
		if (Input::Down(KeyCoord::Key_S)) {
			mTrickRotate.x += 0.05f;
		}
		if (Input::Down(KeyCoord::Key_A)) {
			mTrickRotate.y -= 0.05f;
		}
		if (Input::Down(KeyCoord::Key_D)) {
			mTrickRotate.y += 0.05f;
		}

		mTrickRotate.x -= Input::Analog(PAD_DS4_Velo3Coord::Velo3_Angular).y * 0.1f;
		mTrickRotate.y += Input::Analog(PAD_DS4_Velo3Coord::Velo3_Angular).x * 0.1f;

		mTrickPoint = abs(mTrickRotate.x) + abs(mTrickRotate.y);
		if (mTrickPoint > 0.1f)
		{
			AnimationChange(1, false);
			mTrick = true;
		}

		auto xrotate = XMQuaternionRotationAxis(gameObject->mTransform->Left(), mTrickRotate.x);
		auto yrotate = XMQuaternionRotationAxis(gameObject->mTransform->Up(), mTrickRotate.y);
		/*auto xrotate = XMQuaternionRotationAxis(XMVectorSet(1,0,0,1), mTrickRotate.x);
		auto yrotate = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), mTrickRotate.y);*/
		auto mix = XMQuaternionMultiply(xrotate, yrotate);
		gameObject->mTransform->Quaternion(XMQuaternionMultiply(mix, gameObject->mTransform->Quaternion()));
}

void SailBoard::Jump()
{
	auto physx = gameObject->GetComponent<PhysXComponent>();
	auto force = XMVector3Normalize(physx->GetForceVelocity());
	isJump = true;
	auto v = gameObject->mTransform->Up();
	gameObject->mTransform->AddForce(v*JumpPower,ForceMode::eIMPULSE);
	gameObject->mTransform->AddForce(force, ForceMode::eIMPULSE);
}

bool SailBoard::Dead()
{
	//if (gameObject->mTransform->Position().y < -7) return true;
	auto deadline = game->FindActor("DeadLine");
	if (deadline->mTransform->Position().y > gameObject->mTransform->Position().y)
	{
		return true;
	}
	if (mPlyerHP <= 0)
	{
		return true;
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
			mPlyerHP = 100.0f;
			gameObject->mTransform->Quaternion(XMQuaternionRotationAxis(gameObject->mTransform->Left(), 0));
			auto manager = game->FindActor("PlayerManager")->GetScript<PlayerManager>();
			manager->CreditDown();
			auto physx = gameObject->GetComponent<PhysXComponent>();
			physx->SetForceVelocity(XMVectorSet(0, 0, 0, 1));
			gameObject->mTransform->Position(point->mTransform->Position());
		}
	}
}

bool SailBoard::Shake()
{
	auto f = abs(mPrevAcceler - Input::Analog(PAD_DS4_Velo3Coord::Velo3_Acceleration).z);
	if (f > 0.5f)
	{
		mPrevAcceler = Input::Analog(PAD_DS4_Velo3Coord::Velo3_Acceleration).z;
		return true;
	}
	mPrevAcceler = Input::Analog(PAD_DS4_Velo3Coord::Velo3_Acceleration).z;
	return false;
}

void SailBoard::AnimationChange(int id,bool loop)
{
	auto animator = mBird->GetComponent<AnimationComponent>();
	if (animator)
	{
		auto anima1 = animator->GetAnimetionParam(animator->mCurrentSet);
		auto anima2 = animator->GetAnimetionParam(id);
		anima1.mWeight = 0;
		anima2.mWeight = 1;
		anima1.mTime = 0;
		anima2.mTime = 0;
		anima2.mLoop = loop;
		animator->SetAnimetionParam(animator->mCurrentSet, anima1);
		animator->SetAnimetionParam(id, anima2);
		animator->mCurrentSet = id;
	}
}
