#include "WaterGunBullet.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void WaterGunBullet::Initialize(){
	mSpeed = 4.0f * 0.01f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WaterGunBullet::Start(){

}

//毎フレーム呼ばれます
void WaterGunBullet::Update(){
	mForwardVelocity = gameObject->mTransform->Forward() * mSpeed;
	auto position = gameObject->mTransform->Position();
	gameObject->mTransform->Position(position + mForwardVelocity);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void WaterGunBullet::Finish(){

}

//コライダーとのヒット時に呼ばれます
void WaterGunBullet::OnCollideBegin(Actor* target){
	(void)target;
	if (target->Name() == "Board") {
		game->DestroyObject(gameObject);
	}
}

//コライダーとのヒット中に呼ばれます
void WaterGunBullet::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void WaterGunBullet::OnCollideExit(Actor* target){
	(void)target;
}