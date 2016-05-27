#include "SquidInk.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void SquidInk::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void SquidInk::Start(){
	mDestroyTime *= 60;
}

//毎フレーム呼ばれます
void SquidInk::Update(){
	// 前方に移動
	auto forwardVelocity = gameObject->mTransform->Forward() * -mSpeed * 0.01f;

	auto position = gameObject->mTransform->Position();
	gameObject->mTransform->Position(position + forwardVelocity);


	mDestroyTime--;
	if (mDestroyTime <= 0) {
		game->DestroyObject(gameObject);
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void SquidInk::Finish(){

}

//コライダーとのヒット時に呼ばれます
void SquidInk::OnCollideBegin(Actor* target){

	if (target->Name() == "Board") {

	}
}

//コライダーとのヒット中に呼ばれます
void SquidInk::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void SquidInk::OnCollideExit(Actor* target){
	(void)target;
}