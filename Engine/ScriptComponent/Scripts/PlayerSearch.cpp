#include "PlayerSearch.h"

//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"

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
	// デフォルトの範囲外設定
	mChaseStopDistance = mSizeZ * (5.0f / 3.0f);
}

//毎フレーム呼ばれます
void PlayerSearch::Update(){
	// プレイヤーの捜索
	auto playerObj = game->FindActor("Board");
	if(playerObj == nullptr) mIsPlayerHit = false;
	// プレイヤーが当たったら
	if (mIsPlayerHit) {
		// プレイヤーと親の頭部の距離を求める
		PlayerDistance(playerObj);
		// プレイヤーが索敵範囲から出たら、プレイヤーを見失う
		if (mPlayerDistance >= mChaseStopDistance) {
			/*game->Debug()->Log("敵の索敵範囲外距離:" + std::to_string(mChaseStopDistance));
			game->Debug()->Log("プレイヤーとの距離:" + std::to_string(mPlayerDistance));*/
			mIsPlayerHit = false;
		}
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void PlayerSearch::Finish(){
}

//コライダーとのヒット時に呼ばれます
void PlayerSearch::OnCollideBegin(Actor* target){
	//if (target->Name() == "Wall") {
	//	wallObj = target;
	//	mIsWallHit = true;
	//}

	//if (target->Name() == "Board") {
	//	playerObj = target;
	//	PlayerDistance(target);

	//	// 視野角が壁と当たったら
	//	if (mIsWallHit) {
	//		auto playerLength = XMVector3Length(
	//			playerObj->mTransform->Position() - gameObject->mTransform->Position());
	//		// プレイヤーと敵の間に壁が無かったらtrue
	//		if (!XMVector3InBounds(wallObj->mTransform->Position(), playerLength)) {
	//			mIsPlayerHit = true;
	//		}
	//	}
	//	else {
	//		mIsPlayerHit = true;
	//	}
	//}

	if (target->Name() == "Board") {
		PlayerDistance(target);
		mIsPlayerHit = true;
	}
}

//コライダーとのヒット中に呼ばれます
void PlayerSearch::OnCollideEnter(Actor* target){
	//if (target->Name() == "Wall") {
	//	wallObj = target;
	//	mIsWallHit = true;
	//}

	//if (target->Name() == "Board") {
	//	playerObj = target;
	//	// 視野角が壁と当たったら
	//	if (mIsWallHit) {
	//		auto playerLength = XMVector3Length(
	//			playerObj->mTransform->Position() - gameObject->mTransform->Position());
	//		// プレイヤーと敵の間に壁が無かったらtrue
	//		if (!XMVector3InBounds(wallObj->mTransform->Position(), playerLength)) {
	//			mIsPlayerHit = true;
	//		}
	//	}
	//	else {
	//		mIsPlayerHit = true;
	//	}
	//}

	if (target->Name() == "Board") {
		mIsPlayerHit = true;
	}
}

//コライダーとのロスト時に呼ばれます
void PlayerSearch::OnCollideExit(Actor* target){
	(void)target;
	/*if (target->Name() == "Wall") {
		wallObj = target;
		mIsWallHit = false;
	}*/
}

// スカラー倍した値の取得
void PlayerSearch::SetScalarZ(const float scalarZ) {
	mScalarZ = scalarZ;
}
// プレイヤーとの距離を計算します
void PlayerSearch::PlayerDistance(Actor* playerObj) {
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
	auto targetRange = XMVector3Length(playerObj->mTransform->Position() - parentHeadPoint);
	mPlayerDistance = targetRange.z;
}

// プレイヤーと索敵範囲の始点との距離を返します
float PlayerSearch::GetPlayerDistance() {
	return mPlayerDistance;
}

// プレイヤーに当たったかを返します
bool PlayerSearch::IsPlayerSearch() {
	return mIsPlayerHit;
}
// プレイヤーを追跡中止する距離に加算します
void PlayerSearch::AddChaseStopDistance(float distance) {
	mChaseStopDistance += distance;
}