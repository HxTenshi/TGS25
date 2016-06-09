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
	mInitUpCount = mUpInterval;
	mUpCosine = 0.0f;
	mWallHitCount = 0;
	mInitJampRestTime = mJampRestTime;
	mRotateInterval = 2.0f;
	mSpeed = mSetSpeed;
	mIsJamp = true;
	mIsInitSet = false;
	mIsFloorHit = false;
	mJampVelocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void FlyingFish::Start(){
	Enemy::Start();
	Enemy::SetDamage(mSetDamage);
	Enemy::SetResPawnTime(mSetResPawnTime);
	Enemy::AddPlayerChaseStopDistance(mAddChaseStopDistance);
	Enemy::SetSearchRangeScale(mSetSearchRengeScaleX, mSetSearchRengeScaleY, mSetSearchRengeScaleZ);

	auto floorObj = game->FindActor("Floor");
	mFloorPosition = floorObj->mTransform->Position().y;
}

//毎フレーム呼ばれます
void FlyingFish::Update(){

	Enemy::Move();

	/*if (!mIsInitSet) mInitParentPositionY = -1.5f;*/
	// mIsJampがfalseならジャンプをしていない
	if (mIsJamp == false) {
		// mJampRestTimeが０になったら再度ジャンプする
		if (mJampRestTime <= 0) {
			mJampRestTime = mInitJampRestTime;
		}
		else if (mJampRestTime > 0) {
			mJampRestTime--;
		}

		mUpCount = mUpInterval / 2;
	}
	else {
		// ジャンプ中
		// mUpCountが０になると落ちる
		if (mUpCount > 0) {
			mUpCount--;
		}
		else {
			mUpCount = 0;
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
		// 接触した床の位置を入れる
		mFloorPosition = target->mTransform->Position().y;
		// 床に接触したらtrue
		if (!mIsInitSet && mParentObj->mTransform->Position().y <= mFloorPosition) {
			mIsInitSet = true;
		}
		// 初期の高さ以下となったら再度ジャンプフラグをfalseにする
		if (mParentObj->mTransform->Position().y <= mFloorPosition) {
			mIsJamp = false;
		}

		mIsFloorHit = true;
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
			mUpCosine = 0.0f;
		}
	}
}

//コライダーとのロスト時に呼ばれます
void FlyingFish::OnCollideExit(Actor* target){
	if (target->Name() == "Floor") mIsFloorHit = false;
}

void FlyingFish::SearchMove() {
	auto parentRotate = mParentObj->mTransform->Rotate();
	// 敵の回転
	if (mWallHitCount % 2 == 0) {
		//mRotateY += 3.14f / 180.0f / mRotateInterval;
		parentRotate.y += 3.14f / 180.0f / mRotateInterval;
	}
	else {
		//mRotateY -= 3.14f / 180.0f / mRotateInterval;
		parentRotate.y -= 3.14f / 180.0f / mRotateInterval;
	}

	if (mRotateY >= 3.14f * 2.0f) {
		//mRotateY = 0.0f;
		parentRotate.y = 0.0f;
	}
	mParentObj->mTransform->Rotate(parentRotate);
	
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
		/*if (mUptekitou < 3.141593f / 2.0f) {
			mUptekitou += 3.141593f / mInitUpCount;
		}*/
		mUpCosine += 3.141593f / mInitUpCount;
		mAnimationID = 1;

		if (mUpCount > 0) {
			// 上げる処理
			/*mJampVelocity =
				mParentObj->mTransform->Up() * (mUpPowar + (mUpCount / mUpInterval));*/
			// コサインによるジャンプ
			mJampVelocity = mParentObj->mTransform->Up() * mUpPowar * cosf(mUpCosine) * 0.01f;
			//game->Debug()->Log(std::to_string(cosf(mUpCosine)));
			

			//mJampRestTime = mInitJampRestTime;
			mIsJamp = true;
		}
		else {
			if (parentPosition.y > mFloorPosition) {
				// 落ちる処理
				auto downVelocity = XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);
				/*mJampVelocity = downVelocity * -((mUpPowar)+(mDownCount / mUpInterval));*/

				mJampVelocity = mParentObj->mTransform->Up() * mUpPowar * cosf(mUpCosine) * 0.01f;
				//game->Debug()->Log(std::to_string(cosf(mUpCosine)));

				// 床と接触している場合は絶対に初期値に戻るように補正
				// 接触していなかったら落ちていく
				/*if (mIsFloorHit) {*/
				if (parentPosition.y + mJampVelocity.y < mFloorPosition) {
					if (mIsFloorHit) {
						auto revasionVelocity = XMVectorSet(
							0.0f,
							(float)abs(parentPosition.y) - (float)abs(mFloorPosition),
							0.0f, 0.0f);
						mJampVelocity = revasionVelocity;
					}
					else {
						mJampVelocity = -mParentObj->mTransform->Up() * mUpPowar * 0.01f;
					}	
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
			mAnimationID = 0;
		}
	}
	else {
		// 床まで落ちる
		mParentObj->mTransform->Position(parentPosition - mParentObj->mTransform->Up() * (10.0f * 0.01f));
	}
}