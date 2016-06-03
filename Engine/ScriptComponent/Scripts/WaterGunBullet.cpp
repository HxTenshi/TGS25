#include "WaterGunBullet.h"
#include "SailBoard.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void WaterGunBullet::Initialize(){
	
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WaterGunBullet::Start(){
	mDestroyTime *= 60;


	//mZeroPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//// 新しい水鉄砲の弾の生成
	//mCreateBullet = game->CreateActor("Assets/tgs/WaterShot");
	//game->AddObject(mCreateBullet);
	//mCreateBullet->mTransform->SetParent(gameObject);

	/*auto createBullet = game->CreateActor("Assets/tgs/WaterShot");
	game->AddObject(createBullet);
	createBullet->mTransform->SetParent(gameObject);
	createBullet->mTransform->Rotate(gameObject->mTransform->Up() * (3.14f));*/
}

//毎フレーム呼ばれます
void WaterGunBullet::Update(){
	// 前方に移動
	mForwardVelocity = gameObject->mTransform->Forward() * -mSpeed * 0.01f;

	auto position = gameObject->mTransform->Position();
	gameObject->mTransform->Position(position + mForwardVelocity);

	mSpeed -= 0.01f;

	mDestroyTime--;
	if (mDestroyTime <= 0) {
		game->DestroyObject(gameObject);
	}

	//mCreateBullet->mTransform->Position(mZeroPosition);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void WaterGunBullet::Finish(){
	game->DestroyObject(gameObject);
}

//コライダーとのヒット時に呼ばれます
void WaterGunBullet::OnCollideBegin(Actor* target){

	if (target->Name() == "Board") {
		auto playerScript = target->GetComponent<SailBoard>();
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