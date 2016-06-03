#include "FlyingFish.h"
#include "Enemy.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void FlyingFish::Initialize(){
	Enemy::Initialize();

	mUpCount = mUpInterval / 2;
	mDownCount = 0;
	mWallHitCount = 0;
	mInitJampRestTime = mJampRestTime;
	mRotateInterval = 2.0f;
	mSpeed = mSetSpeed;
	mIsJamp = true;
	mIsInitSet = false;
	mJampVelocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void FlyingFish::Start(){
	Enemy::Start();

	Enemy::SetDamage(mSetDamage);
	Enemy::SetResPawnTime(mSetResPawnTime);
	//Enemy::EnemyCGCreate();

	auto floorObj = game->FindActor("Floor");
	mFloorPosition = floorObj->mTransform->Position().y;
	/*mInitParentPositionY = mParentObj->mTransform->Position().y;*/
}

//毎フレーム呼ばれます
void FlyingFish::Update(){

	Enemy::Move();

	/*if (!mIsInitSet) mInitParentPositionY = -1.5f;*/

	if (mIsJamp == false) {
		if (mJampRestTime <= 0) {
			mJampRestTime = mInitJampRestTime;
		}
		else if (mJampRestTime > 0) {
			mJampRestTime--;
		}

		mUpCount = mUpInterval / 2;
		mDownCount = 0;
	}
	else {
		if (mUpCount > 0) {
			mUpCount--;
		}
		else {
			mUpCount = 0;
			if (mDownCount < 30) {
				mDownCount++;
			}
		}
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void FlyingFish::Finish(){
	Enemy::Finish();

	mUpCount = 0;
}

//コライダーとのヒット時に呼ばれます
void FlyingFish::OnCollideBegin(Actor* target){
	Enemy::OnCollideBegin(target);
	if (target->Name() == "Floor") {
		// 床に接触したらtrue
		if (!mIsInitSet && mParentObj->mTransform->Position().y <= mFloorPosition) {
			mIsInitSet = true;
		}
		// 初期の高さ以下となったら再度ジャンプフラグをfalseにする
		if (mParentObj->mTransform->Position().y <= mFloorPosition) {
			mIsJamp = false;
		}
	}

	if (target->Name() == "Wall") {
		mWallHitCount++;
		mRotateY =  mRotateY + 3.14f - (3.14f / 180.0f / mRotateInterval);
	}
}

//コライダーとのヒット中に呼ばれます
void FlyingFish::OnCollideEnter(Actor* target){
	Enemy::OnCollideEnter(target);

	if (target->Name() == "Floor") {
		if (!mIsInitSet && mParentObj->mTransform->Position().y <= mFloorPosition) {
			mIsInitSet = true;
		}

		if (mParentObj->mTransform->Position().y <= mFloorPosition) {
			mIsJamp = false;
		}
	}
}

//コライダーとのロスト時に呼ばれます
void FlyingFish::OnCollideExit(Actor* target){
	Enemy::OnCollideExit(target);
}

void FlyingFish::SearchMove() {

	// 敵の回転
	if (mWallHitCount % 2 == 0) {
		mRotateY += 3.14f / 180.0f / mRotateInterval;
	}
	else {
		mRotateY -= 3.14f / 180.0f / mRotateInterval;
	}

	if (mRotateY >= 3.14f * 2.0f) {
		mRotateY = 0.0f;
	}
	auto rotate = XMVectorSet(0.0f, mRotateY, 0.0f, 0.0f);
	mParentObj->mTransform->Rotate(rotate);
	
	JampMove();
}

void FlyingFish::PlayerChase() {
	Enemy::PlayerChaseMode();
	JampMove();

}

void FlyingFish::JampMove() {
	auto parentPosition = mParentObj->mTransform->Position();
	auto forwardMove = mParentObj->mTransform->Forward() * -mSpeed * 0.01f;

	// ジャンプ間隔
	if (mJampRestTime <= 0) {
		if (mUpCount > 0) {
			// 上げる処理
			mJampVelocity =
				mParentObj->mTransform->Up() * (mUpPowar + (mUpCount / mUpInterval));

			mIsJamp = true;
		}
		else {
			if (parentPosition.y > mFloorPosition) {
				// 落ちる処理
				auto downVelocity = XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);
				mJampVelocity = downVelocity * -((mUpPowar)+(mDownCount / mUpInterval));

				// 絶対に初期値に戻るように補正
				if (parentPosition.y + mJampVelocity.y < mFloorPosition) {
					auto revasionVelocity = XMVectorSet(
						0.0f,
						(float)abs(parentPosition.y) - (float)abs(mFloorPosition)
						, 0.0f, 0.0f);
					mJampVelocity = revasionVelocity;
					//game->Debug()->Log(std::to_string(mJampVelocity.y));
				}
			}
		}
	}
	// 初期段階で床と接触していなかったら
	if (mIsInitSet) {
		// 接触したらジャンプする
		if (mIsJamp) {
			mParentObj->mTransform->Position(parentPosition + forwardMove + mJampVelocity);
		}
		else {
			mParentObj->mTransform->Position(parentPosition + forwardMove);
		}
	}
	else {
		// 床まで落ちる
		mParentObj->mTransform->Position(parentPosition - mParentObj->mTransform->Up() * (10.0f * 0.01f));
	}
}