#include "EnemyCG.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
//#include "Game/Component/AnimationComponent.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void EnemyCG::Initialize(){
	mAnimationID = 0;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void EnemyCG::Start(){

}

//毎フレーム呼ばれます
void EnemyCG::Update(){
	//game->DestroyObject(gameObject);

	/*auto animation = gameObject->GetComponent<AnimationComponent>();
	animation->ChangeAnimetion(mAnimationID);*/
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void EnemyCG::Finish(){

}

//コライダーとのヒット時に呼ばれます
void EnemyCG::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void EnemyCG::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void EnemyCG::OnCollideExit(Actor* target){
	(void)target;
}

void EnemyCG::ChangeAnimation(int id) {

}