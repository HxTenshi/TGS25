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

	//mUpInterval *= 60;
	mUpCount = mUpInterval / 2;
	mDownCount = 0;
	//mJampRestTime *= 60;
	mWallHitCount = 0;
	mInitJampRestTime = mJampRestTime;
	//mUpPowar *= 0.01f;
	mRotateInterval = 2.0f;
	mFloorPosition = 0.0f;
	mSpeed = mSetSpeed;
	mIsJamp = true;
	mIsInitSet = false;
	mJampVelocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void FlyingFish::Start(){
	//game->Debug()->Log(std::to_string(mSize));
	Enemy::Start();
	mInitPositionY = mParentObj->mTransform->Position().y;

	Enemy::setDamage(mSetDamage);
}

//毎フレーム呼ばれます
void FlyingFish::Update(){

	if (!mIsInitSet) mInitPositionY = -1.5f;

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

	Enemy::Move();
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void FlyingFish::Finish(){
	mUpCount = 0;
}

//コライダーとのヒット時に呼ばれます
void FlyingFish::OnCollideBegin(Actor* target){
	Enemy::OnCollideBegin(target);
	if (target->Name() == "Floor") {
		if (!mIsInitSet && mParentObj->mTransform->Position().y <= mInitPositionY) {
			mIsInitSet = true;
		}
		

		if (mParentObj->mTransform->Position().y <= mInitPositionY) {
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
		if (!mIsInitSet && mParentObj->mTransform->Position().y <= mInitPositionY) {
			mIsInitSet = true;
		}

		if (mParentObj->mTransform->Position().y <= mInitPositionY) {
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
		mRotateY += 3.14 / 180;
	}
	else {
		mRotateY -= 3.14 / 180 / mRotateInterval;
	}

	if (mRotateY >= 3.14 * 2.0f) {
		mRotateY = 0.0f;
	}
	auto rotate = XMVectorSet(0.0f, mRotateY, 0.0f, 0.0f);
	mParentObj->mTransform->Rotate(rotate);
	
	JampMove();

	/*gameObject->mTransform->Position(position + v + mJampVelocity);*/

	//mParentObj->mTransform->Position(parentPosition + forwardMove + mJampVelocity);
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
			if (parentPosition.y > mInitPositionY) {
				// 落ちる処理
				auto downVelocity = XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);
				mJampVelocity = downVelocity * -((mUpPowar)+(mDownCount / mUpInterval));

				// 絶対に初期値に戻るように補正
				if (parentPosition.y + mJampVelocity.y < mInitPositionY) {
					auto revasionVelocity = XMVectorSet(
						0.0f,
						(float)abs(parentPosition.y) - (float)abs(mInitPositionY)
						, 0.0f, 0.0f);
					mJampVelocity = revasionVelocity;
					//game->Debug()->Log(std::to_string(mJampVelocity.y));
				}
			}
		}
	}

	if (mIsInitSet) {
		if (mIsJamp) {
			mParentObj->mTransform->Position(parentPosition + forwardMove + mJampVelocity);
		}
		else {
			mParentObj->mTransform->Position(parentPosition + forwardMove);
		}
	}
	else {
		mParentObj->mTransform->Position(parentPosition - mParentObj->mTransform->Up() * (10.0f * 0.01f));
	}
}