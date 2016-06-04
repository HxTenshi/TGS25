#include "PointItem.h"

#include "Input/Input.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"PhysX\IPhysXEngine.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void PointItem::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void PointItem::Start(){

}

//毎フレーム呼ばれます
void PointItem::Update()
{
	auto player = game->FindActor("Board");
	auto v = player->mTransform->Position() - gameObject->mTransform->Position();
	if (XMVector3Length(v).x < 5)
	{
		v = XMVector3Normalize(v);
		gameObject->mTransform->Position(gameObject->mTransform->Position() + (v * 0.1f));
	}

	mAngel += 0.01f;
	gameObject->mTransform->Quaternion(XMQuaternionRotationAxis(gameObject->mTransform->Up(), mAngel));
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void PointItem::Finish(){

}

//コライダーとのヒット時に呼ばれます
void PointItem::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void PointItem::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void PointItem::OnCollideExit(Actor* target){
	(void)target;
}