#include "Squid.h"
#include "Enemy.h"
#include "PlayerSearch.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"
#include "Input/Input.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void Squid::Initialize(){
	Enemy::Initialize();

	mInitMoveInterval = mMoveInterval;
	mInitWaitInterval = mWaitInterval;

	mSetDamage = 0;
	mSpeed = mSetSpeed;
	mIsPlayerHit = false;
	mSRPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Squid::Start(){
	Enemy::Start();
	Enemy::SetDamage(mSetDamage);
	Enemy::SetResPawnTime(mSetResPawnTime);
	Enemy::AddPlayerChaseStopDistance(mAddChaseStopDistance);
}

//毎フレーム呼ばれます
void Squid::Update(){
	Enemy::Move();
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Squid::Finish(){
	Enemy::Finish();
}

//コライダーとのヒット時に呼ばれます
void Squid::OnCollideBegin(Actor* target){
	Enemy::OnCollideBegin(target);
	
	if (target->Name() == "Board") {
		mIsPlayerHit = true;
	}
}

//コライダーとのヒット中に呼ばれます
void Squid::OnCollideEnter(Actor* target){
	Enemy::OnCollideEnter(target);
}

//コライダーとのロスト時に呼ばれます
void Squid::OnCollideExit(Actor* target){
	Enemy::OnCollideExit(target);
}

void Squid::SearchMove() {

	// 親を動かす
	auto parentPosition = mParentObj->mTransform->Position();
	auto forwardMove = mParentObj->mTransform->Forward() * (mSpeed * 0.01f);

	if (mMoveInterval > 0) {
		mParentObj->mTransform->Position(parentPosition - forwardMove);
		mMoveInterval--;
	}
	else {
		if (mWaitInterval > 0) {
			mWaitInterval--;
		}
		else {
			mMoveInterval = mInitMoveInterval;
			mWaitInterval = mInitWaitInterval;
		}
	}
}

void Squid::PlayerChase() {	

	// プレイヤーの方向を向く（まだ）
	auto playerObj = game->FindActor("Board");
	auto playPo = playerObj->mTransform->Position();
	// 親のステータスの取得
	auto parPo = mParentObj->mTransform->Position();
	auto parRo = mParentObj->mTransform->Rotate();

	// プレイヤーと当たったら、プレイヤーに巻き付く
	if (mIsPlayerHit) {
		// twineAround
		auto playerScale = playerObj->mTransform->Scale();
		auto twineAroundPosition = XMVectorSet(playPo.x, playPo.y + (playerScale.y / 2.0f), playPo.z, 0.0f);
		mParentObj->mTransform->Position(twineAroundPosition);
		// 操作に支障をきたすスクリプトを呼ぶ
	}
}