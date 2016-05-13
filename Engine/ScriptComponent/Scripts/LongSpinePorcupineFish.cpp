#include "LongSpinePorcupineFish.h"
#include "Enemy.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Engine\DebugEngine.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void LongSpinePorcupineFish::Initialize(){
	Enemy::Initialize();

	/*mSize = gameObject->mTransform->Scale().z;*/
	mSpeed = 2.0f;

	mSwellUpCount = 0;
	mSwellUpTimer = 300;
	mDirection = 1.0f;
	mSwellUpSize = 1.5;
	mIsPlayerKill = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void LongSpinePorcupineFish::Start(){

}

//毎フレーム呼ばれます
void LongSpinePorcupineFish::Update(){

	auto v = XMVectorSet(mSize, mSize, mSize, mSize);

	gameObject->mTransform->Scale(v);

	// 色の設定
	auto color = XMFLOAT4(1, 1, 1, 1);
	// 一定距離内だと色の値を変える
	if (mIsSearchRange == true) {
		//100 = 1秒？ 
		if (mSwellUpCount % mSwellUpTimer < mSwellUpTimer / 2) {
			mSize += 1.5f / mSwellUpTimer / 2;
			color = XMFLOAT4(1, 0, 0, 1);
			mIsPlayerKill = true;
		}
		else {
			mSize -= 1.5f / mSwellUpTimer / 2;
			color = XMFLOAT4(0, 1, 0, 1);
			mIsPlayerKill = false;
		}
	}
	else {
		color = XMFLOAT4(1, 1, 1, 1);
	}
	// 色の更新
	/*auto mate = gameObject->GetComponent<MaterialComponent>();*/ // ->SetAlbedoColor(color);
	auto mate = gameObject->GetComponent<MaterialComponent>();
	if (mate) {
		mate->SetAlbedoColor(color);
	}

	Enemy::PlayerSpaceChase(gameObject, 60 * 10, mSwellUpCount);

	if (!mIsSearchRange) {
		mSwellUpCount = 0;
		mSize = 1.0f;
		mDirection += 1.0f;
		SearchMove();
	}
	else {
		mSwellUpCount++;
	}

	/*Enemy::PlayerChase(gameObject);*/

	Enemy::FallDead(gameObject);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void LongSpinePorcupineFish::Finish(){

}

//コライダーとのヒット時に呼ばれます
void LongSpinePorcupineFish::OnCollideBegin(Actor* target){
	(void)target;
	/*if (target->Name() == "Board") {
		if (mIsPlayerKill) {
			game->DestroyObject(target);
		}		
	}*/
	if (target->Name() == "Wall") {
		//mDirection = -mDirection;
		//game->Debug()->Log(std::to_string(mDirection));
	}
}

//コライダーとのヒット中に呼ばれます
void LongSpinePorcupineFish::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void LongSpinePorcupineFish::OnCollideExit(Actor* target){
	(void)target;
}

void LongSpinePorcupineFish::SearchMove() {
	auto x = -1;
	auto z = -1;
	if ((int)mDirection % 360 <= 90 || (int)mDirection % 360 >= 270) {
		x = 1;
	}
	if ((int)mDirection % 360 > 180) {
		z = -1;
	}

	auto v = XMVectorSet(x, 0, z, 0);
	auto r = sinf(mDirection*(3.14f / 180));
	//gameObject->mTransform->Position(v*mSpeed);
	//gameObject->mTransform->AddForce(v*mSpeed*1.5f*r);
}