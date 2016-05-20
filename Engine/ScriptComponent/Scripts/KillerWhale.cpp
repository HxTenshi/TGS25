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
	
	mSpeed = 4.0f * 0.01f;
	mMotionCount = 0;
	mIsShot = false;
	//// 不死身判定
	//mIsImmortalBody = true;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void KillerWhale::Start(){
	Enemy::Start();
}

//毎フレーム呼ばれます
void KillerWhale::Update(){
	//Enemy::PlayerColorChange(gameObject);
	Enemy::PlayerSearchMode(gameObject);
	Enemy::PlayerChaseMode(gameObject);
	//Enemy::PlayerChase(gameObject);
	Enemy::FallDead(gameObject);
	//game->Debug()->Log(std::to_string(Enemy::PlayerDistance()));

	//game->Debug()->Log(std::to_string(mIsSearchRange));
	auto PlayerObj = game->FindActor("Board");

	mMotionCount++;
	if (mMotionCount >= 60 * 3) {
		if (!mIsShot) {
			// 水鉄砲の弾の生成
			auto GunBullet = game->CreateActor("Assets/WaterGunBullet");
			game->AddObject(GunBullet);
			// 位置の変更
			Enemy::SetForwardObj(GunBullet);
		}
		mIsShot = true;
	}

	//auto thisPosi = gameObject->mTransform->Position();
	//thisPosi = XMVector3Normalize(thisPosi);
	//auto otherPosi = PlayerObj->mTransform->Position();
	//otherPosi = XMVector3Normalize(otherPosi);
	////auto radius = XMVector3AngleBetweenVectors(thisPosi, otherPosi);
	//auto radius = XMVector3AngleBetweenNormals(otherPosi , thisPosi);
	////auto angle = XMConvertToDegrees(radius);
	//auto radiusZ = radius.z * 180 / (float)XM_PI;

	//game->Debug()->Log(std::to_string(radiusZ));
	
	// 後でvector で行動の配列を作り、 size判定も余りを出してifを少なくさせる
	if (mIsSearchRange) {
		if (Enemy::PlayerDistance() <= mSize.z * 1.0f + mHalfSizeZ ||
			Enemy::PlayerDistance() <= mSize.z * 2.0f + mHalfSizeZ) {
			mMotionCount++;
			//game->Debug()->Log("発射");
			if (mMotionCount >= 60 * 3) {
				if (!mIsShot) {
					// 水鉄砲の弾の生成
					auto GunBullet = game->CreateActor("Assets/WaterGunBullet");
					game->AddObject(GunBullet);
					// 位置の変更
					Enemy::SetForwardObj(GunBullet);
				}
				mIsShot = true;
			}
			else if (mMotionCount >= 60 * 5) {
				Enemy::PlayerChase(gameObject);
			}
		}
		else if (Enemy::PlayerDistance() > mSize.z * 2.0f + mHalfSizeZ ||
			Enemy::PlayerDistance() <= mSize.z * 3.0f + mHalfSizeZ) {
			//game->Debug()->Log("まだ");
			Enemy::PlayerChase(gameObject);
		}
	}

	auto objPosition = gameObject->mTransform->Position();
	auto forwardMove = gameObject->mTransform->Forward() * mSpeed;
	// Y軸の補正 (敵の一部が海面に出ているようにする)
	if (mIsFloorHit) {	
		auto setPosition = XMVectorSet(objPosition.x, mPositionY, objPosition.z, 0.0f);
		gameObject->mTransform->Position(setPosition + forwardMove);
	}
	else {
		gameObject->mTransform->Position(objPosition + forwardMove);
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void KillerWhale::Finish(){
	//Enemy::Finish();
}

//コライダーとのヒット時に呼ばれます
void KillerWhale::OnCollideBegin(Actor* target){
	(void)target;
	Enemy::OnCollideBegin(target);
}

//コライダーとのヒット中に呼ばれます
void KillerWhale::OnCollideEnter(Actor* target){
	(void)target;
	Enemy::OnCollideEnter(target);
}

//コライダーとのロスト時に呼ばれます
void KillerWhale::OnCollideExit(Actor* target){
	(void)target;
	Enemy::OnCollideExit(target);
}