#include "Enemy.h"
#include "PlayerSearch.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Engine\DebugEngine.h"

Enemy::~Enemy() {
	//gameObject->mTransform->AllChildrenDestroy();
}

//生成時に呼ばれます（エディター中も呼ばれます）
void Enemy::Initialize(){
	mAddSearchObjCount = 0;
	mSpeed = 1.0f * 0.01f;
	mSize = gameObject->mTransform->Scale();
	mHalfSizeZ = mSize.z / 2.0f;
	mPositionY = gameObject->mTransform->Position().y;
	mRotateY = 0.0f;
	mDeadHeigth = -10.0f;
	mIsObjHit = false;
	mIsFloorHit = false;
	mIsSearchRange = false;
	mIsImmortalBody = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Enemy::Start(){
	//game->Debug()->Log(std::to_string(mSize));

	auto nowPosition = gameObject->mTransform->Position();
	mPositionY = nowPosition.y;
}

//毎フレーム呼ばれます
void Enemy::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Enemy::Finish(){
	/*game->DestroyObject(mPlayerSearchObj);*/
}

//コライダーとのヒット時に呼ばれます
void Enemy::OnCollideBegin(Actor* target){
	if (target->Name() == "Board"){
		if (!mIsSearchRange && !mIsImmortalBody) {
			game->DestroyObject(gameObject);
		}
		mIsObjHit = true;
	}
}

//コライダーとのヒット中に呼ばれます
void Enemy::OnCollideEnter(Actor* target){
	if (target->Name() == "Board") {
		mIsObjHit = true;
	}

	if (target->Name() == "Floor") {
		mIsFloorHit = true;
	}
}

//コライダーとのロスト時に呼ばれます
void Enemy::OnCollideExit(Actor* target){
	if (target->Name() == "Board") {
		mIsObjHit = false;
	}

	if (target->Name() == "Floor") {
		mIsFloorHit = false;
	}
}

void Enemy::PlayerColorChange(Actor* obj) {
	// 色の設定
	auto color = XMFLOAT4(1, 1, 1, 1);
	// 一定距離内だと色の値を変える
	if (mIsSearchRange) {
		color = XMFLOAT4(1, 0, 0, 1);
	}
	else {
		color = XMFLOAT4(1, 1, 1, 1);
	}
	// 色の更新
	auto mate = gameObject->GetComponent<MaterialComponent>();
	if (mate) {
		//game->Debug()->Log(std::to_string(color.y));
		mate->SetAlbedoColor(color);
	}
}

void Enemy::SearchMove() {
	auto v = gameObject->mTransform->Forward() * mSpeed;
	auto position = gameObject->mTransform->Position();
	gameObject->mTransform->Position(position + v);

	mRotateY += 3.14 / 180;
	
	auto rotate = XMVectorSet(0.0f, mRotateY, 0.0f, 0.0f);

	gameObject->mTransform->Rotate(rotate);
}

void Enemy::PlayerSearchMode(Actor* thisObj, const float sizeX, const float sizeY, const float sizeZ) {
	// プレイヤーの捜索
	/*auto object = game->FindActor("Board");
	if (!object) return;*/

	// 索敵範囲オブジェの作成
	if (mAddSearchObjCount < 1) {
		mPlayerSearchObj = game->CreateActor("Assets/PlayerSearchRange");
		game->AddObject(mPlayerSearchObj);
		// 敵の子供に追加する
		mPlayerSearchObj->mTransform->SetParent(gameObject);
		// 索敵オブジェのスクリプト取得
		mPlayerSearchObj->GetScript<PlayerSearch>();
		// 大きさの変更
		auto objScale = gameObject->mTransform->Scale();
		auto setScale = XMVectorSet(sizeX, sizeY, sizeZ, 0.0f);
		mPlayerSearchObj->mTransform->Scale(setScale);
		// 目の前の位置に設置する
		Enemy::SetForwardChildrenObj(mPlayerSearchObj);

		// 一度生成したら再度生成しない
		mAddSearchObjCount = 1;
	}

	searchScript = mPlayerSearchObj->GetScript<PlayerSearch>();

	if (searchScript->IsPlayerSearch()) {
		mIsSearchRange = true;
		//game->Debug()->Log("当たり");
	}
	else {
		mIsSearchRange = false;
	}
}

void Enemy::PlayerSearchMode(Actor* thisObj) {
	// 索敵範囲オブジェの作成
	if (mAddSearchObjCount < 1) {
		mPlayerSearchObj = game->CreateActor("Assets/PlayerSearchRange");
		game->AddObject(mPlayerSearchObj);
		// 敵の子供に追加する
		mPlayerSearchObj->mTransform->SetParent(gameObject);
		// 索敵オブジェのスクリプト取得
		searchScript = mPlayerSearchObj->GetScript<PlayerSearch>();
		// 大きさの変更
		auto objScale = gameObject->mTransform->Scale();
		auto setScale = XMVectorSet(objScale.x, objScale.y * 2.0f, 3.0f, 0.0f);
		mPlayerSearchObj->mTransform->Scale(setScale);
		// 目の前の位置に設置する
		Enemy::SetForwardChildrenObj(mPlayerSearchObj);

		// 一度生成したら再度生成しない
		mAddSearchObjCount = 1;
	}

	//searchScript = mPlayerSearchObj->GetScript<PlayerSearch>();


	if (searchScript->IsPlayerSearch()) {
		mIsSearchRange = true;
		//game->Debug()->Log("当たり");
	}
	else {
		mIsSearchRange = false;
	}
}

void  Enemy::PlayerChaseMode(Actor* thisObj) {
	// プレイヤーの捜索
	auto object = game->FindActor("Board");
	if (!object) {
		return;
	}
	else {
		// プレイヤーとの距離の計算
		auto target = ((object->mTransform->Position()) - thisObj->mTransform->Position());
		// 正規化
		target = XMVector3Normalize(target);

		// 移動
		if (mIsObjHit == false) {

			if (mIsSearchRange) {
				//// プレイヤーの方向を向く
				auto mRotate = XMVectorSet(target.x, 0.0f, target.z, 0.0f);
				
				gameObject->mTransform->Quaternion(mRotate);

				//thisObj->mTransform->Quaternion(target);
				/*auto rotateZero = XMVectorSet(0.0f, gameObject->mTransform->Rotate().y, 0.0f, 0.0f);
				gameObject->mTransform->Rotate(rotateZero);*/

				/*auto thisObj = gameObject->mTransform->Position();
				auto player = object->mTransform->Position();
				auto addVector = thisObj + player;
				auto rotateY = 180.0f / 3.14f;
				auto objRotate = XMVectorSet(0.0f, rotateY, 0.0f, 0.0f);
				if (addVector.x < 0) {
					gameObject->mTransform->Rotate(gameObject->mTransform->Rotate() + -objRotate);
				}
				else if (addVector.x > 0) {
					gameObject->mTransform->Rotate(gameObject->mTransform->Rotate() + objRotate);
				}*/
			}

		}
	}
}

void Enemy::PlayerChase(Actor* thisObj) {
	// 移動
	if (!mIsObjHit) {
		auto moveSpeed = thisObj->mTransform->Forward() * -mSpeed;
		auto movePosition = thisObj->mTransform->Position();
		// Y軸の補正 (敵の一部が海面に出ているようにする)
		if (mIsFloorHit) {
			auto setPosition = XMVectorSet(movePosition.x, mPositionY, movePosition.z, 0.0f);
			gameObject->mTransform->Position(setPosition + moveSpeed);
		}
		else {
			thisObj->mTransform->Position(movePosition + moveSpeed);
		}
	}
}

// 間隔移動
void Enemy::PlayerSpaceChase(Actor* thisObj, const int interval, int count) {
	// プレイヤーの捜索
	auto object = game->FindActor("Board");
	if (!object) return;

	auto target = ((object->mTransform->Position()) - thisObj->mTransform->Position());
	auto targetRange = XMVector3Length(object->mTransform->Position() - thisObj->mTransform->Position());

	target = XMVector3Normalize(target);

	//target.y = 0;

	// 移動
	if (!mIsObjHit) {
		// 一定距離なら反応する
		if (targetRange.x <= (mSize.z / 2) + (mSize.z * 6)) {
			if (count % interval < interval / 2) {
				gameObject->mTransform->AddForce(target*mSpeed);
			}
			mIsSearchRange = true;
		}
		else if (targetRange.x >(mSize.z / 2) + (mSize.z * 6)) {
			gameObject->mTransform->Forward() * 0;
			mIsSearchRange = false;
		}
	}
}

// 目の前にオブジェを生成します
void Enemy::SetForwardObj(Actor* setObj) {
	auto objPosition = gameObject->mTransform->Position();
	auto objScale = gameObject->mTransform->Scale();
	auto setObjScale = setObj->mTransform->Scale();
	// 位置の指定
	auto setPosition = XMVectorSet(0.0f, 0.0f, ((1.0f - objScale.z) / 2.0f) + (objScale.z + setObjScale.z) / 2.0f, 0.0f);
	// 位置の変更
	setObj->mTransform->Position(objPosition + -setPosition);
	// 生成元のオブジェの回転角に変更
	setObj->mTransform->Rotate(gameObject->mTransform->Rotate());
}

// 親のオブジェの前に子のオブジェを作成します
void Enemy::SetForwardChildrenObj(Actor* setObj) {
	// 索敵範囲オブジェの位置設定
	auto objScale = gameObject->mTransform->Scale();
	auto childrenScale = setObj->mTransform->Scale();
	// 位置の指定
	auto setPosition = XMVectorSet(0.0f, 0.0f, ((1.0f - objScale.z) / 2.0f) + (objScale.z + childrenScale.z) / 2.0f, 0.0f);
	// 位置の変更
	setObj->mTransform->Position(-setPosition);
}

// 一定距離まで落ちたら死亡します
void Enemy::FallDead(Actor* thisObj) {
	// 一定の位置まで落ちたら死亡
	if (thisObj->mTransform->Position().y <= mDeadHeigth) {
		game->DestroyObject(thisObj);
	}
}

// プレイヤーとの距離の計算 (Z座標)
float Enemy::PlayerDistance() {
	// プレイヤーの検索
	auto object = game->FindActor("Board");
	//if (!object) return 0.0f;
	// プレイヤーとの距離の計算
	auto targetRange = XMVector3Length(
		object->mTransform->Position() - gameObject->mTransform->Position());
	return targetRange.z;
}