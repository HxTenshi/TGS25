#include "Sail.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"
#include "Engine\DebugEngine.h"
#include "SailBoard.h"
#include"Game\Component\MaterialComponent.h"
#include"h_standard.h"
#include"PlayerManager.h"

#include"Wind.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void Sail::Initialize(){
	mSailRotate = 0.00f;
	mVelocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	move = 0;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Sail::Start(){
	
	
}

//毎フレーム呼ばれます
void Sail::Update(){
	

	SailRotate();
	auto parent = gameObject->mTransform->GetParent()->mTransform->GetParent();
	auto forward = parent->mTransform->Forward();
	forward.y = 0;
	forward = XMVector3Normalize(forward);

	auto velocity = parent->GetComponent<PhysXComponent>()->GetForceVelocity();
	velocity.x = 0;
	velocity.z = 0;

	if (!parent->GetScript<SailBoard>()->GetIsJump() && parent->GetScript<SailBoard>()->GetIsGround())
	{
		move += MovePower() * Speed * game->DeltaTime()->GetDeltaTime();
	}
	move -= move * 0.9f * game->DeltaTime()->GetDeltaTime();
	mVelocity.y = -5 * (60 * game->DeltaTime()->GetDeltaTime());
	move = min(max(move, 0), 100);
	
	if (!parent->GetScript<SailBoard>()->GetIsJump() && parent->GetScript<SailBoard>()->GetIsGround())
	{
		parent->GetComponent<PhysXComponent>()->SetForceVelocity(velocity + (forward * move));
	}


	auto manager = game->FindActor("PlayerManager")->GetScript<PlayerManager>();
	if (!manager->IsGameStart())
	{
		parent->GetComponent<PhysXComponent>()->SetForceVelocity(XMVectorSet(0, 0, 0, 0));
	}
	//if (!parent->GetScript<SailBoard>()->GetIsJump()) parent->mTransform->AddForce(parent->mTransform->Forward() * MovePower() * Speed);
	//parent->mTransform->AddForce(parent->mTransform->Forward() * MovePower() * 5);         //PCデバック

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Sail::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Sail::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void Sail::OnCollideEnter(Actor* target){

}

//コライダーとのロスト時に呼ばれます
void Sail::OnCollideExit(Actor* target){
	(void)target;
}

//風の向き＋セイルの向きで推進力の計算 返り値
float Sail::MovePower()
{

	auto wind = gameObject->mTransform->GetParent()->mTransform->GetParent()->GetScript<SailBoard>();
	if (!wind)return 0.0f;
	auto windvec = wind->GetWind();
	windvec.y = 0;

	auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), mSailRotate);
	gameObject->mTransform->Quaternion(rotatey);

	auto saildir = gameObject->mTransform->Left();

	saildir = XMVector3Normalize(saildir);
	if (XMVector3Length(windvec).x < 0.000000001f) {
		return 0.0f;
	}
	windvec = XMVector3Normalize(windvec);

	auto rad = XMVector3Dot(windvec, saildir);

	return abs(rad.x);
}

float Sail::GetSailRotateRad()
{
	return mSailRotate;
}

void Sail::SailRotate()
{
	if (Input::Down(KeyCoord::Key_O) && mSailRotate > -XM_PI / 2) {
		mSailRotate -= 0.05f;
	}
	if (Input::Down(KeyCoord::Key_P) && mSailRotate < XM_PI / 2) {
		mSailRotate += 0.05f;
	}

	mSailRotate += Input::Analog(PAD_DS4_Velo3Coord::Velo3_Angular).z;

	mSailRotate = min(max(mSailRotate, -XM_PI / 2), XM_PI / 2);

	if (Input::Down(PAD_DS4_KeyCoord::Button_CROSS)) mSailRotate = 0;
}
