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
	mIsBlowAway = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void KillerWhale::Start(){
	Enemy::Start();
	Enemy::SetDamage(mSetDamage);
	Enemy::SetResPawnTime(mSetResPawnTime);
	Enemy::AddPlayerChaseStopDistance(mAddChaseStopDistance);
	Enemy::SetSearchRangeScale(mSetSearchRengeScaleX, mSetSearchRengeScaleY, mSetSearchRengeScaleZ);
	Enemy::SetTornadoStatus(mSetTornadoPower, mSetTornadoBlowAwayInterval, mSetTornadoDistance);

	mInitBulletShotTime = mBulletShotTime;
	mInitRecastTime = mRecastTime;
	mGRAVITY = mParentObj->mTransform->Up() * 0.098f;
}

//毎フレーム呼ばれます
void KillerWhale::Update(){
	Enemy::Move();	
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
	Enemy::SetAnimationID(0);
	Enemy::SetAnimationTimeScale(1.0f * mSpeed * 0.2f);
	Enemy::SetAnimationLoop(true);
}

void KillerWhale::ShortDistanceAttack() {

	mIsAttckMode = true;
	//mAnimationID = 1;
	Enemy::SetAnimationID(1);
	//Enemy::SetAnimationLoop(false);

	//game->Debug()->Log("発射");
	if (mBulletShotTime <= 0) {
		if (!mIsShot) {
			// 水鉄砲の弾の生成
			auto gunBullet = game->CreateActor("Assets/Enemy/WaterGunBullet");
			game->AddObject(gunBullet);
			// 位置の変更
			Enemy::SetParentForwardObj(gunBullet);

			Enemy::SetAnimationTimeScale(1.0f);

			mIsShot = true;
			Enemy::SetAnimationLoop(false);
		}
		else {
			mRecastTime--;
			if (mRecastTime <= 0) {
				mBulletShotTime = mInitBulletShotTime;
				mRecastTime = mInitRecastTime;
				mIsShot = false;
				mIsAttckMode = false;
				Enemy::SetAnimationLoop(true);
			}
		}	
	}
	else {
		// 発射時間が0になるまでプレイヤーの方向を向く
		mBulletShotTime--;
		Enemy::PlayerChaseMode(0.0f, 0.0f);
		Enemy::SetAnimationTimeScale(2.0f * (35.0f / mInitBulletShotTime));
	}

}

void KillerWhale::CenterDistanceAttack() {
	ShortDistanceAttack();
}

void KillerWhale::LongDistanceAttack() {
	Enemy::PlayerChaseMode(0.0f, 0.0f);

	Enemy::SetAnimationID(0);
	Enemy::SetAnimationLoop(true);

	auto parentPosition = mParentObj->mTransform->Position();
	auto forwardMove = mParentObj->mTransform->Forward() * mSpeed * 0.01f;
	if (!mIsFloorHit) {
		forwardMove += mGRAVITY;
	}
	mParentObj->mTransform->Position(parentPosition - forwardMove);
}