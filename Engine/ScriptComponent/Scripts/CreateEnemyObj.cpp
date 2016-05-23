#include "CreateEnemyObj.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Engine\DebugEngine.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void CreateEnemyObj::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void CreateEnemyObj::Start(){
	mInitCreateInterval = mCreateInterval;
	mInitCreateDelayTime = mCreateDelayTime;
}

//毎フレーム呼ばれます
void CreateEnemyObj::Update(){
	auto playerObj = game->FindActor("Board");
	auto playerPosition = playerObj->mTransform->Position();
	auto playerRotate = playerObj->mTransform->Rotate();
	auto thisPosition = gameObject->mTransform->Position();
	auto distance = XMVector3Length(playerPosition - thisPosition);

	if (mCreateObj == NULL) {
		mCreateObj = game->CreateActor("Assets/FlyingFish");
	}

	if (distance.z <= mReactionDistance) {

		if (mCreateInterval <= 0) {

			if (!mIsCreateObj && mCreateCount > 0) {
				// 敵オブジェの生成
				game->AddObject(mCreateObj);
				mCreateObj->mTransform->Position(thisPosition);
				mCreateObj->mTransform->Rotate(playerRotate);

				mIsCreateObj = true;
			}
			
			mCreateDelayTime--;
			
			if (mCreateDelayTime <= 0) {
				// 各パラメータの初期化
				mCreateInterval = mInitCreateInterval;
				mCreateDelayTime = mInitCreateDelayTime;
				mCreateCount--;
				mIsCreateObj = false;
			}			
		}
		else {
			mCreateInterval--;
		}
	
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void CreateEnemyObj::Finish(){

}

//コライダーとのヒット時に呼ばれます
void CreateEnemyObj::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void CreateEnemyObj::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void CreateEnemyObj::OnCollideExit(Actor* target){
	(void)target;
}