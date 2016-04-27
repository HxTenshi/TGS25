#include "Wind.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void Wind::Initialize(){

	mWindVelocity = XMVectorSet(1, 0, 0, 1);

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Wind::Start(){

}

//毎フレーム呼ばれます
void Wind::Update()
{
	/*auto obj = game->FindActor("Board");
	if (obj)
	{
		obj->mTransform->AddForce(mWindVelocity);
	}*/
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Wind::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Wind::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void Wind::OnCollideEnter(Actor* target){
	/*if (target->Name() == "Board")
	{
		target->mTransform->AddForce(mWindVelocity);
	}*/
}

//コライダーとのロスト時に呼ばれます
void Wind::OnCollideExit(Actor* target){
	(void)target;
}

XMVECTOR Wind::GetWindVelocity(){
	return mWindVelocity;
}