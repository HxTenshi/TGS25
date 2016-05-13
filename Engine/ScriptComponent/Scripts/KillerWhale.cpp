#include "KillerWhale.h"
#include "Enemy.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void KillerWhale::Initialize(){
	Enemy::Initialize();

	/*mSize = gameObject->mTransform->Scale().z;*/
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void KillerWhale::Start(){

}

//毎フレーム呼ばれます
void KillerWhale::Update(){
	Enemy::PlayerColorChange(gameObject);
	Enemy::PlayerSearchMode(gameObject);
	//Enemy::PlayerChaseMode(gameObject);
	Enemy::PlayerChase(gameObject);
	Enemy::FallDead(gameObject);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void KillerWhale::Finish(){

}

//コライダーとのヒット時に呼ばれます
void KillerWhale::OnCollideBegin(Actor* target){
	(void)target;
	Enemy::OnCollideBegin(target);
}

//コライダーとのヒット中に呼ばれます
void KillerWhale::OnCollideEnter(Actor* target){
	(void)target;
	Enemy::OnCollideEnter(target);
}

//コライダーとのロスト時に呼ばれます
void KillerWhale::OnCollideExit(Actor* target){
	(void)target;
	Enemy::OnCollideExit(target);
}