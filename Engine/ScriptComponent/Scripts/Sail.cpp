#include "Sail.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"
#include "Engine\DebugEngine.h"

#include"Wind.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void Sail::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Sail::Start(){
	mRotateY = 0.00f;
}

//毎フレーム呼ばれます
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

//風の向き＋セイルで推進力の計算
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
