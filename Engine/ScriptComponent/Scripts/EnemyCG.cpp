#include "EnemyCG.h"
#include "Enemy.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Engine\DebugEngine.h"
//#include "Game/Component/AnimationComponent.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void EnemyCG::Initialize(){
	mAnimationID = 0;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void EnemyCG::Start(){
	auto enemyObj = gameObject->mTransform->GetParent();
	mEnemyScript = enemyObj->GetScript<Enemy>();
}

//毎フレーム呼ばれます
void EnemyCG::Update(){
	/*if (mAnimationID != mEnemyScript->GetAnimationID()) {
		mAnimationID = mEnemyScript->GetAnimationID();
		auto animation = gameObject->GetComponent<AnimationComponent>();
		animation->ChangeAnimetion(mAnimationID);
	}*/
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