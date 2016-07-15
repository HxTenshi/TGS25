#include "FlyingFish.h"
#include "Enemy.h"

//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"
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
	mIsWallHit = false;
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
	Enemy::SetTornadoStatus(
		mSetTornadoPower, mSetTornadoRotateScale, mSetAddTornadoRotateScale,
		mSetTornadoRotatePower, mSetTornadoUpPower, mSetTornadoInterval,
		mSetTornadoDistance);

	auto floorObj = game->FindActor("Floor");
	mFloorPosition = floorObj->mTransform->Position().y + 0.9f;
}

//毎フレーム呼ばれます
void FlyingFish::Update(){
	Enemy::Move();
	// ジャンプ行動
	JampMove();
	// mIsJampがfalseならジャンプをしていない
	if (mIsJamp == false) {
		// mJampRestTimeが０になったら再度ジャンプする
		if (mJampRestTime <= 0) {
			mJampRestTime = mInitJampRestTime;
		}
		else if (mJampRestTime > 0) {
			mJampRestTime -= Enemy::GetEnemyDeltaTime(60.0f);
		}

		mUpCount = mUpInterval / 2;
	}
	else {
		// ジャンプ中
		Enemy::SetAnimationID(1);
		Enemy::SetAnimationTimeScale(2.0f * (23.0f / mInitUpCount));
		Enemy::SetAnimationLoop(false);
		// mUpCountが０になると落ちる
		if (mUpCount > 0) {
			mUpCount -= Enemy::GetEnemyDeltaTime(60.0f);
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
		mFloorPosition = mParentObj->mTransform->Position().y ;
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

	if (target->Name() == "Tower" ||
		target->Name() == "bridge" || 
		target->Name() == "Tree") {
		mWallHitCount++;
		mIsWallHit = true;
		auto parentRotate = mParentObj->mTransform->Rotate();
		parentRotate.y += 3.141593f;
		mParentObj->mTransform->Rotate(parentRotate);
	}
}

//コライダーとのヒット中に呼ばれます
void FlyingFish::OnCollideEnter(Actor* target){
	Enemy::OnCollideEnter(target);

	if (target->Name() == "Floor") {
		if (mParentObj->mTransform->Position().y <= mFloorPosition) {
			if (!mIsInitSet) {
				mIsInitSet = true;
			}
			mUpCosine = 0.0f;
			mIsJamp = false;
		}
		Enemy::SetAnimationID(0);
	}
}

//コライダーとのロスト時に呼ばれます
void FlyingFish::OnCollideExit(Actor* target){
	if (target->Name() == "Floor") mIsFloorHit = false;
}

void FlyingFish::PlayerSearchMove() {
	auto parentRotate = mParentObj->mTransform->Rotate();
	// 敵の回転
	if (mWallHitCount % 2 == 0) 
		parentRotate.y += (3.14f / 180.0f / mRotateInterval)
		* Enemy::GetEnemyDeltaTime(60.0f);
	else 
		parentRotate.y -= (3.14f / 180.0f / mRotateInterval)
		* Enemy::GetEnemyDeltaTime(60.0f);

	if (parentRotate.y >= 3.141593f * 2.0f)
		parentRotate.y -= 3.141593f * 2.0f;
	else if (parentRotate.y < 0.0f)
		parentRotate.y += 3.141593f * 2.0f;
	if (mIsInitSet)
		mParentObj->mTransform->Rotate(parentRotate);
}

void FlyingFish::PlayerChaseMove() {
	Enemy::PlayerChaseMode(5.0f, 2.0f);
}

void FlyingFish::JampMove() {
	auto parentPosition = mParentObj->mTransform->Position();
	auto forwardMove = mParentObj->mTransform->Forward() * -mSpeed * 0.01f;
	// ジャンプ間隔
	if (mJampRestTime <= 0) {
		mUpCosine += (3.141593f / mInitUpCount) * Enemy::GetEnemyDeltaTime(60.0f);
		if (mUpCosine > 3.141593f) mUpCosine = 3.141593f;
		if (mUpCount > 0) {
			// 上げる処理
			// コサインによるジャンプ
			//mJampVelocity = (mParentObj->mTransform->Up() * mUpPowar * cosf(mUpCosine) * 0.01f);
			auto velocityY = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			mJampVelocity = (velocityY * mUpPowar * cosf(mUpCosine) * 0.01f);
			mIsJamp = true;
		}
		else {
			//if (parentPosition.y > mFloorPosition) {
				// 落ちる処理
				auto downVelocity = XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);
				mJampVelocity = (mParentObj->mTransform->Up() * mUpPowar * cosf(mUpCosine) * 0.01f);
				// 床と接触している場合は絶対に初期値に戻るように補正
				// 接触していなかったら落ちていく
				if (parentPosition.y + mJampVelocity.y < mFloorPosition) {
					if (mIsFloorHit) {
						auto revasionVelocity = XMVectorSet(
							0.0f,
							(float)abs(parentPosition.y) - (float)abs(mFloorPosition),
							0.0f, 0.0f);
						mJampVelocity = revasionVelocity;
					}
					else {
						//mJampVelocity = (-mParentObj->mTransform->Up() * mUpPowar * 0.01f);
						auto velocityY = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
						mJampVelocity = (velocityY * mUpPowar  * cosf(mUpCosine) * 0.01f);
					}
				}
			//}
		}
	}
	// 初期段階で床と接触していなかったら
	if (mIsInitSet) {
		// 接触したらジャンプする
		if (mIsJamp) {
			mParentObj->mTransform->Position(
				(parentPosition + (forwardMove + mJampVelocity) *
					Enemy::GetEnemyDeltaTime(60.0f)));
		}
		else {
			mParentObj->mTransform->Position(
				(parentPosition + forwardMove * 
					Enemy::GetEnemyDeltaTime(60.0f)));
		}
	}
	else {
		// 床まで落ちる
		mParentObj->mTransform->Position(
			(parentPosition - (mParentObj->mTransform->Up() * (10.0f * 0.01f)
				* Enemy::GetEnemyDeltaTime(60.0f))));
	}
}