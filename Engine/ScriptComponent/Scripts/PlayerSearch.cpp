#include "PlayerSearch.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Engine\DebugEngine.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void PlayerSearch::Initialize(){
	mIsPlayerHit = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void PlayerSearch::Start(){
	mSizeZ = gameObject->mTransform->Scale().z;
	game->Debug()->Log(std::to_string(mSizeZ));
}

//毎フレーム呼ばれます
void PlayerSearch::Update(){
	if (mIsPlayerHit) {
		// プレイヤーの捜索
		auto object = game->FindActor("Board");
		if (!object) return;

		// プレイヤーとの距離の計算
		auto targetRange = XMVector3Length(
			object->mTransform->Position() - gameObject->mTransform->Position());

		// 親の取得
		auto parentObj = gameObject->mTransform->GetParent();
		auto parentSize = parentObj->mTransform->Scale();

		/*if (targetRange.z > mSizeZ * (5 / 3)) {
			game->Debug()->Log("hit");
			mIsPlayerHit = false;
		}*/
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void PlayerSearch::Finish(){

}

//コライダーとのヒット時に呼ばれます
void PlayerSearch::OnCollideBegin(Actor* target){
	if (target->Name() == "Board") {
		game->Debug()->Log("当たり221e23131r2d");
		mIsPlayerHit = true;
	}
}

//コライダーとのヒット中に呼ばれます
void PlayerSearch::OnCollideEnter(Actor* target){

	if (target->Name() == "Board") {
		game->Debug()->Log("当たりd12d12fd1");
		mIsPlayerHit = true;
	}
}

//コライダーとのロスト時に呼ばれます
void PlayerSearch::OnCollideExit(Actor* target){
	(void)target;
}

bool PlayerSearch::IsPlayerSearch() {
	return mIsPlayerHit;
}