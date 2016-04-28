#include "Enemy.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Engine\DebugEngine.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void Enemy::Initialize(){
	mSpeed = 4.0f;
	mSize = 4;
	mIsHit = false;
	mIsSearchRange = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Enemy::Start(){
	//auto color = XMFLOAT4(0, 0, 0, 1);

	//auto mate = gameObject->GetComponent<MaterialComponent>();// ->SetAlbedoColor(color);
	//if (mate) {
	//	mate->SetAlbedoColor(color);
	//}
}

//毎フレーム呼ばれます
void Enemy::Update(){
	

	PlayerSearch();
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Enemy::Finish(){

}

void Enemy::PlayerSearch() {
	// 色の設定
	auto color = XMFLOAT4(1, 1, 1, 1);
	// 一定距離内だと色の値を変える
	if (mIsSearchRange == true) {
		color = XMFLOAT4(1, 0, 0, 1);
	}
	else {
		color = XMFLOAT4(1, 1, 1, 1);
	}
	// 色の更新
	auto mate = gameObject->GetComponent<MaterialComponent>();// ->SetAlbedoColor(color);
	if (mate) {
		mate->SetAlbedoColor(color);
	}

	// プレイヤーの捜索
	auto obj = game->FindActor("Board");
	if (!obj) return;

	auto target = ((obj->mTransform->Position()) - gameObject->mTransform->Position());
	auto targetRange = XMVector3Length(obj->mTransform->Position() - gameObject->mTransform->Position());

	target = XMVector3Normalize(target);
	//target.y = 0;

	// 移動
	if (mIsHit == true) {
		//gameObject->mTransform->AddForce(target * 0);
		gameObject->mTransform->Forward() * 0;
		//gameObject->mTransform->Position() -= target;
	}
	else {
		//game->Debug()->Log(std::to_string(targetRange.x));
		// 一定距離なら反応する
		if (targetRange.x < mSize * 3) {
			gameObject->mTransform->AddForce(target*mSpeed);
			mIsSearchRange = true;
		}
		else if (targetRange.x >= mSize * 5) {
			mIsSearchRange = false;
		}
	}
}

//コライダーとのヒット時に呼ばれます
void Enemy::OnCollideBegin(Actor* target){
	(void)target;
	if (target->Name() == "Board"){
		mIsHit = true;
	}
}

//コライダーとのヒット中に呼ばれます
void Enemy::OnCollideEnter(Actor* target){
	(void)target;
	if (target->Name() == "Board") {
		mIsHit = true;
	}
}

//コライダーとのロスト時に呼ばれます
void Enemy::OnCollideExit(Actor* target){
	(void)target;
	mIsHit = false;
}