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
	
	mSpeed = mSetSpeed;
	mIsShot = false;
	mIsDistanceAct = true;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void KillerWhale::Start(){
	Enemy::Start();
	Enemy::SetDamage(mSetDamage);
	Enemy::SetResPawnTime(mSetResPawnTime);
	Enemy::AddPlayerChaseStopDistance(mAddChaseStopDistance);

	mInitBulletShotTime = mBulletShotTime;
	mInitRecastTime = mRecastTime;
	mGRAVITY = mParentObj->mTransform->Up() * 0.098f;
}

//毎フレーム呼ばれます
void KillerWhale::Update(){
	Enemy::Move();
	//game->Debug()->Log(std::to_string(mIsFloorHit));
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void KillerWhale::Finish(){
	Enemy::Finish();
}

//コライダーとのヒット時に呼ばれます
void KillerWhale::OnCollideBegin(Actor* target){
	Enemy::OnCollideBegin(target);
}

//コライダーとのヒット中に呼ばれます
void KillerWhale::OnCollideEnter(Actor* target){
	Enemy::OnCollideEnter(target);
}

//コライダーとのロスト時に呼ばれます
void KillerWhale::OnCollideExit(Actor* target){
	Enemy::OnCollideExit(target);
}

void KillerWhale::SearchMove() {
	// 何もしない
	//game->Debug()->Log(std::to_string(mIsFloorHit));
	if (!mIsFloorHit && mInitSetCount == 1) {
		auto parentPosition = mParentObj->mTransform->Position();
		mParentObj->mTransform->Position(parentPosition - mGRAVITY);
	}
}

void KillerWhale::ShortDistanceAttack() {

	mIsAttckMode = true;
	mAnimationID = 1;

	// 発射時間が0になるまでプレイヤーの方向を向く
	mBulletShotTime--;
	//game->Debug()->Log("発射");
	if (mBulletShotTime <= 0) {
		if (!mIsShot) {
			// 水鉄砲の弾の生成
			auto gunBullet = game->CreateActor("Assets/Enemy/WaterGunBullet");
			game->AddObject(gunBullet);
			// 位置の変更
			Enemy::SetParentForwardObj(gunBullet);

			mIsShot = true;
		}
		else {
			mRecastTime--;
			if (mRecastTime <= 0) {
				mBulletShotTime = mInitBulletShotTime;
				mRecastTime = mInitRecastTime;
				mIsShot = false;
				mIsAttckMode = false;
			}
		}	
	}
	else {
		Enemy::PlayerChaseMode();
	}

}

void KillerWhale::CenterDistanceAttack() {
	ShortDistanceAttack();
}

void KillerWhale::LongDistanceAttack() {
	Enemy::PlayerChaseMode();

	mAnimationID = 1;
	auto parentPosition = mParentObj->mTransform->Position();
	auto forwardMove = mParentObj->mTransform->Forward() * mSpeed * 0.01f;
	if (!mIsFloorHit) {
		forwardMove += mGRAVITY;
	}
	mParentObj->mTransform->Position(parentPosition - forwardMove);
}