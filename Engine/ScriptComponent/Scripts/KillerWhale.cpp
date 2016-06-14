#include "KillerWhale.h"
#include "Enemy.h"

//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"
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
	Enemy::SetTornadoStatus(
		mSetTornadoPower, mSetTornadoRotateScale, mSetAddTornadoRotateScale,
		mSetTornadoRotatePower, mSetTornadoUpPower, mSetTornadoDistance);

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
	mIsMove = false;
	//game->Debug()->Log(std::to_string(mIsFloorHit));
	if (!mIsFloorHit && mInitSetCount == 1) {
		auto parentPosition = mParentObj->mTransform->Position();
		mParentObj->mTransform->Position(parentPosition - mGRAVITY * Enemy::GetEnemyDeltaTime(60.0f));
	}
	Enemy::SetAnimationID(0);
	Enemy::SetAnimationTimeScale(1.0f * mSpeed * 0.2f);
	Enemy::SetAnimationLoop(true);
}

void KillerWhale::ShortDistanceAttack() {
	mIsMove = false;
	// 攻撃モードをオンにする
	mIsAttckMode = true;
	Enemy::SetAnimationID(1);
	// カウントが０になったら水鉄砲の弾を発射
	if (mBulletShotTime <= 0) {
		if (!mIsShot) {
			//// 水鉄砲の弾の生成
			auto gunBullet = game->CreateActor("Assets/Enemy/WaterGunBullet");
			game->AddObject(gunBullet);
			//// 位置の変更
			Enemy::SetParentForwardObj(gunBullet);
			mIsShot = true;
			Enemy::SetAnimationTimeScale(1.0f);
			//Enemy::SetAnimationLoop(false);
		}
		else {
			//mRecastTime -= deltaTime * 30;
			mRecastTime -= Enemy::GetEnemyDeltaTime(30.0f);
			Enemy::SetAnimationTimeScale(2.0f * (15.0f / mInitRecastTime));
			if (mRecastTime <= 0) {
				mBulletShotTime = mInitBulletShotTime;
				mRecastTime = mInitRecastTime;
				mIsShot = false;
				mIsAttckMode = false;
				Enemy::SetAnimationTime(0.0f);
				//Enemy::SetAnimationLoop(true);
			}
		}	
	}
	else {
		// 発射時間が0になるまでプレイヤーの方向を向く
		//mBulletShotTime--;
		mBulletShotTime -= Enemy::GetEnemyDeltaTime(30.0f);
		Enemy::PlayerChaseMode(0.0f, 0.0f);
		Enemy::SetAnimationTimeScale(2.0f * (34.0f / mInitBulletShotTime));
		//game->Debug()->Log(std::to_string(Enemy::GetAnimationTime()));
	}

	if (Enemy::GetAnimationTime() >= 40.0f) {
		Enemy::SetAnimationLoop(false);
	}
	else {
		Enemy::SetAnimationLoop(true);
	}
	// Enemy::SetAnimationLoop(true);
}

void KillerWhale::CenterDistanceAttack() {
	ShortDistanceAttack();
}

void KillerWhale::LongDistanceAttack() {
	mIsMove = true;
	Enemy::PlayerChaseMode(0.0f, 0.0f);

	Enemy::SetAnimationID(0);
	Enemy::SetAnimationLoop(true);
	auto parentPosition = mParentObj->mTransform->Position();
	auto forwardMove = mParentObj->mTransform->Forward() * mSpeed * 0.01f;
	// 床に当たっていないなら落ちる(あとで書き換える)
	if (!mIsFloorHit) {
		forwardMove += mGRAVITY;
	}
	
	mParentObj->mTransform->Position((parentPosition - forwardMove * Enemy::GetEnemyDeltaTime(60.0f)));
}