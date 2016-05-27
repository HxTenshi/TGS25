#include "LongSpinePorcupineFish.h"
#include "Enemy.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Engine\DebugEngine.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void LongSpinePorcupineFish::Initialize(){
	Enemy::Initialize();
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void LongSpinePorcupineFish::Start(){

}

//毎フレーム呼ばれます
void LongSpinePorcupineFish::Update(){
	Enemy::Move();
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void LongSpinePorcupineFish::Finish(){

}

//コライダーとのヒット時に呼ばれます
void LongSpinePorcupineFish::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void LongSpinePorcupineFish::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void LongSpinePorcupineFish::OnCollideExit(Actor* target){
	(void)target;
}