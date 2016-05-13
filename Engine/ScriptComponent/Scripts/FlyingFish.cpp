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

	mUpInterval = 60 * 1;
	mUpCount = mUpInterval / 2;
	mDownCount = 0;
	mJampRestTime = 60;
	mWallHitCount = 0;
	mInitJampRestTime = mJampRestTime;
	mUpPowar = 4.0f * 0.01f;
	mInitPositionY = gameObject->mTransform->Position().y;
	mRotateInterval = 2.0f;
	mIsJamp = true;
	mJampVelocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void FlyingFish::Start(){
	//game->Debug()->Log(std::to_string(mSize));
}

//毎フレーム呼ばれます
void FlyingFish::Update(){

	auto v = gameObject->mTransform->Forward() * -mSpeed;
	auto position = gameObject->mTransform->Position();

	Enemy::PlayerColorChange(gameObject);
	Enemy::PlayerSearchMode(gameObject);
	//Enemy::PlayerChase(gameObject);

	if (mIsJamp == false) {
		//game->Debug()->Log(std::to_string(mJampRestTime));
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

	// 敵の回転
	if (!mIsSearchRange) {
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
		gameObject->mTransform->Rotate(rotate);
	}
	else {
		//Enemy::PlayerChaseMode(gameObject);
	}
	// ジャンプ間隔
	if(mJampRestTime <= 0){
		if (mUpCount > 0) {
			// 上げる処理
			mJampVelocity = gameObject->mTransform->Up() * (mUpPowar + (mUpCount / mUpInterval));

			mIsJamp = true;
		}
		else {
			if (position.y > mInitPositionY) {
				auto upVelocity = XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);
				mJampVelocity = upVelocity * -((mUpPowar) + (mDownCount / mUpInterval));
				//mJampVelocity = gameObject->mTransform->Up() * -((mUpPowar / 10) + (mDownCount / mUpInterval));
			}
		}
	}
	
	/*gameObject->mTransform->Position(position + v + mJampVelocity);*/
	if (mIsJamp) {
		gameObject->mTransform->Position(position + v + mJampVelocity);
	}
	else {
		gameObject->mTransform->Position(position + v);
	}

	Enemy::FallDead(gameObject);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void FlyingFish::Finish(){
	mUpCount = 0;
}

//コライダーとのヒット時に呼ばれます
void FlyingFish::OnCollideBegin(Actor* target){
	(void)target;
	Enemy::OnCollideBegin(target);

	if (target->Name() == "Wall") {
		mWallHitCount++;
		mRotateY =  mRotateY + 3.14f - (3.14f / 180.0f / mRotateInterval);
	}
}

//コライダーとのヒット中に呼ばれます
void FlyingFish::OnCollideEnter(Actor* target){
	(void)target;
	Enemy::OnCollideEnter(target);

	if (target->Name() == "Floor") {
		//if (mPositionY - target->mTransform->Position().y <= 0) {
			//game->Debug()->Log(std::to_string(mPositionY - target->mTransform->Position().y));
			mIsJamp = false;
		//}
	}
}

//コライダーとのロスト時に呼ばれます
void FlyingFish::OnCollideExit(Actor* target){
	(void)target;
	Enemy::OnCollideExit(target);
}