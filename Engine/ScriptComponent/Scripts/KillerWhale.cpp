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
	//EnemyCGCreate("Shachi/shachi", 0.1f, 0.1f, 0.1f);

	/*mBulletShotTime *= 60;
	mRecastTime *= 60;*/
	mInitBulletShotTime = mBulletShotTime;
	mInitRecastTime = mRecastTime;

	//Enemy::EnemyCGCreate();
}

//毎フレーム呼ばれます
void KillerWhale::Update(){
	/*if (mParentCreateCount == 1) {

		if (mCGCreateCount == 0) {
			Enemy::EnemyCGCreate();
			mCGCreateCount = 1;
		}
	}*/

	Enemy::Move();

	//game->Debug()->Log(std::to_string(mIsAttckMode));
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

}

void KillerWhale::ShortDistanceAttack() {

	mIsAttckMode = true;

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

	//if (!mIsAttckMode) {
		auto parentPosition = mParentObj->mTransform->Position();
		auto forwardMove = mParentObj->mTransform->Forward() * mSpeed * 0.01f;
		// Y軸の補正 (敵の一部が海面に出ているようにする)
		if (mIsFloorHit) {
			/*auto setPosition = XMVectorSet(objPosition.x, mPositionY, objPosition.z, 0.0f);
			gameObject->mTransform->Position(setPosition + forwardMove);*/
		}
		else {
			mParentObj->mTransform->Position(parentPosition - forwardMove);
		}
	//}
	/*else {
		ShortDistanceAttack();
	}*/
}