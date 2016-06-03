#include "PlayerSearch.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXColliderComponent.h"
#include "Engine\DebugEngine.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void PlayerSearch::Initialize(){
	mPlayerDistance = 0.0f;
	mIsPlayerHit = false;
	mIsWallHit = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void PlayerSearch::Start(){
	mSizeZ = gameObject->mTransform->Scale().z;
	auto collider = gameObject->GetComponent<PhysXColliderComponent>();
	collider->SetScale(gameObject->mTransform->Scale());
	//game->Debug()->Log(std::to_string(mSizeZ));
}

//毎フレーム呼ばれます
void PlayerSearch::Update(){
	// プレイヤーの捜索
	auto object = game->FindActor("Board");
	if(!object) mIsPlayerHit = false;

	if (mIsPlayerHit) {
		// 親の取得
		auto parentObj = gameObject->mTransform->GetParent();
		auto parentPosition = parentObj->mTransform->Position();
		auto parentRotate = parentObj->mTransform->Rotate();

		// 親の頭部にベクトルを設定する
		auto parentHeadPoint = XMVectorSet(
			parentPosition.x - ((mSizeZ / mScalarZ) / 2.0f * sinf(parentRotate.y)),
			parentPosition.y,
			parentPosition.z - ((mSizeZ / mScalarZ) / 2.0f * cosf(parentRotate.y)),
			0.0f);
		
		// プレイヤーと親の頭部の距離の計算
		auto targetRange = XMVector3Length(object->mTransform->Position()- parentHeadPoint);

		mPlayerDistance = targetRange.z;
		//game->Debug()->Log(std::to_string(targetRange.z));

		if (mPlayerDistance >= mSizeZ * (5.0f / 3.0f)) {
			//game->Debug()->Log("いない");
			game->Debug()->Log(std::to_string(mPlayerDistance));
			game->Debug()->Log(std::to_string(mSizeZ * (5.0f / 3.0f)));
			mIsPlayerHit = false;
		}
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void PlayerSearch::Finish(){

}

//コライダーとのヒット時に呼ばれます
void PlayerSearch::OnCollideBegin(Actor* target){
	if (target->Name() == "Wall") {
		wallObj = target;
		mIsWallHit = true;
	}

	if (target->Name() == "Board") {
		//game->Debug()->Log("当たり221e23131r2d");
		playerObj = target;
		// 視野角が壁と当たったら
		if (mIsWallHit) {
			auto playerLength = XMVector3Length(
				playerObj->mTransform->Position() - gameObject->mTransform->Position());
			// プレイヤーと敵の間に壁が無かったらtrue
			if (!XMVector3InBounds(wallObj->mTransform->Position(), playerLength)) {
				mIsPlayerHit = true;
			}
		}
		else {
			mIsPlayerHit = true;
		}
	}
}

//コライダーとのヒット中に呼ばれます
void PlayerSearch::OnCollideEnter(Actor* target){
	if (target->Name() == "Wall") {
		wallObj = target;
		mIsWallHit = true;
	}

	if (target->Name() == "Board") {
		//game->Debug()->Log("当たり221e23131r2d");
		playerObj = target;
		// 視野角が壁と当たったら
		if (mIsWallHit) {
			auto playerLength = XMVector3Length(
				playerObj->mTransform->Position() - gameObject->mTransform->Position());
			// プレイヤーと敵の間に壁が無かったらtrue
			if (!XMVector3InBounds(wallObj->mTransform->Position(), playerLength)) {
				mIsPlayerHit = true;
			}
		}
		else {
			mIsPlayerHit = true;
		}
	}
}

//コライダーとのロスト時に呼ばれます
void PlayerSearch::OnCollideExit(Actor* target){
	(void)target;
	if (target->Name() == "Wall") {
		wallObj = target;
		mIsWallHit = false;
	}
}

// スカラー倍した値の取得
void PlayerSearch::SetScalarZ(const float scalarZ) {
	mScalarZ = scalarZ;
}

// プレイヤーと索敵範囲の始点との距離を返します
float PlayerSearch::PlayerDistance() {
	return mPlayerDistance;
}

// プレイヤーに当たったかを返します
bool PlayerSearch::IsPlayerSearch() {
	return mIsPlayerHit;
}