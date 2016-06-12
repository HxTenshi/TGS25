#include "WaterGunBullet.h"
#include "SailBoard.h"

//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void WaterGunBullet::Initialize(){
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WaterGunBullet::Start(){
}

//毎フレーム呼ばれます
void WaterGunBullet::Update(){
	// 前方に移動
	mForwardVelocity = gameObject->mTransform->Forward() * -mSpeed * 0.01f;

	auto position = gameObject->mTransform->Position();
	gameObject->mTransform->Position(position + mForwardVelocity);

	mDestroyTime--;
	if (mDestroyTime <= 0) {
		game->DestroyObject(gameObject);
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void WaterGunBullet::Finish(){
	gameObject->mTransform->AllChildrenDestroy();
	game->DestroyObject(gameObject);
}

//コライダーとのヒット時に呼ばれます
void WaterGunBullet::OnCollideBegin(Actor* target){
	// プレイヤーに当たったら消す
	if (target->Name() == "Board") {
		auto playerScript = target->GetScript<SailBoard>();
		playerScript->Damage(mSetDamege);
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