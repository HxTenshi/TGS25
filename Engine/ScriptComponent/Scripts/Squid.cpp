#include "Squid.h"
#include "Enemy.h"
#include "PlayerSearch.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void Squid::Initialize(){
	Enemy::Initialize();

	mInitMoveInterval = mMoveInterval;
	mInitWaitInterval = mWaitInterval;

	mCreatCount = 0;
	mSRPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Squid::Start(){
	Enemy::Start();
	Enemy::setDamage(mSetDamage);
}

//毎フレーム呼ばれます
void Squid::Update(){
	auto parentObj = gameObject->mTransform->GetParent();
	//auto p = parentObj->GetComponent<TransformComponent>();

	auto objPosition = parentObj->mTransform->Position();
	auto parentScale = parentObj->mTransform->Scale();
	//auto forwardMove = parentObj->mTransform->Forward() * 4.0f;

	//auto squidObj = game->FindActor("SquidParent");

	if (mCreatCount == 0) {
		//Enemy::PlayerSearchMode(parentObj);
		mPlayerSearchObj = game->CreateActor("Assets/PlayerSearchRange");
		game->AddObject(mPlayerSearchObj);

		// 索敵オブジェのスクリプト取得
		searchScript = mPlayerSearchObj->GetScript<PlayerSearch>();
		// 大きさの変更
		auto objScale = gameObject->mTransform->Scale();
		auto setScale = XMVectorSet(objScale.x, objScale.y * 2.0f, objScale.z * 3.0f, 0.0f);
		mPlayerSearchObj->mTransform->Scale(setScale);

		// 索敵範囲オブジェの位置設定
		auto childrenScale = mPlayerSearchObj->mTransform->Scale();
		// 位置の指定
		auto setPosition = XMVectorSet(0.0f, 0.0f, (objScale.z + childrenScale.z) / 2.0f, 0.0f);
		mSRPosition = setPosition;

		// 位置の変更
		mPlayerSearchObj->mTransform->Position(-setPosition);
		game->Debug()->Log(std::to_string(mPlayerSearchObj->mTransform->Position().z));

		// 敵の子供に追加する
		mPlayerSearchObj->mTransform->SetParent(parentObj);

		mCreatCount = 1;
	}

	if (searchScript->IsPlayerSearch()) {
		mIsSearchRange = true;
		//game->Debug()->Log("当たり");
	}
	else {
		mIsSearchRange = false;
	}

	//game->Debug()->Log(std::to_string(mPlayerSearchObj->mTransform->Position().z));

	// 索敵オブジェの位置更新
	mPlayerSearchObj->mTransform->Position(-mSRPosition);

	if (parentObj->Name() == "SquidParent") {
		ParentMove();

		//auto squidPosition = squidObj->mTransform->Position();		

		/*auto pRotate = parentObj->mTransform->Rotate();
		auto pRotateY = 3.141593f / 180.0f;
		auto rotateV = XMVectorSet(0.0f, pRotateY, 0.0f, 0.0f);
		parentObj->mTransform->Rotate(pRotate + rotateV);*/

		// プレイヤーの方向を向く（まだ）
		if (mIsSearchRange) {
			auto playerObj = game->FindActor("Board");
			auto playPo = playerObj->mTransform->Position();

			auto parPo = parentObj->mTransform->Position();
			auto parRo = parentObj->mTransform->Rotate();
			auto YzeroPlayerPosi = XMVectorSet(playPo.x, parPo.y, playPo.z, 0.0f);
			//auto parAngle = parRo.y * 57.3f;
			auto setForPo = XMVectorSet(parPo.x - (20.0f * sinf(parRo.y)),
				parPo.y, parPo.z - (20.0f * cosf(parRo.y)), 0.0f);

			auto pRotate = parentObj->mTransform->Rotate();
			auto pRotateY = 3.141593f / 180.0f / 10.0f;
			auto rotateV = XMVectorSet(0.0f, pRotateY, 0.0f, 0.0f);
			auto distance = XMVector3LinePointDistance(
				parPo,
				setForPo,
				YzeroPlayerPosi);

			auto nomalizeSetForPosi = XMVector3Normalize(setForPo);
			auto nomalizeYZERO = XMVector3Normalize(YzeroPlayerPosi);

			auto nomalizeAdd = (nomalizeSetForPosi.x * nomalizeYZERO.x)
				+ (nomalizeSetForPosi.z * nomalizeYZERO.z);

			//game->Debug()->Log(std::to_string(sinf(pRotate.y)));
			//game->Debug()->Log(std::to_string(distance.y));
			//game->Debug()->Log(std::to_string(setForPo.x - playPo.x));
			game->Debug()->Log(std::to_string(cosf(nomalizeAdd)));
			//game->Debug()->Log(std::to_string(parPo.z - (20.0f * cosf(parRo.y))));

			// 値の加算
			/*if (distance.y > 0.01f) {

				if (setForPo.x - playPo.x < 0) {
					parentObj->mTransform->Rotate(pRotate - rotateV);
				}
				else if (setForPo.x - playPo.x > 0) {
					parentObj->mTransform->Rotate(pRotate + rotateV);
				}
			}*/
			
		}
	}
	else {
		game->Debug()->Log("見つからない");
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Squid::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Squid::OnCollideBegin(Actor* target){
	Enemy::OnCollideBegin(target);
}

//コライダーとのヒット中に呼ばれます
void Squid::OnCollideEnter(Actor* target){
	Enemy::OnCollideEnter(target);
}

//コライダーとのロスト時に呼ばれます
void Squid::OnCollideExit(Actor* target){
	Enemy::OnCollideExit(target);
}

void Squid::ParentMove() {
	// 親を動かす
	auto parentPosition = mParentObj->mTransform->Position();
	auto forwardMove = mParentObj->mTransform->Forward() * (mSpeed * 0.01f);

	if (mMoveInterval > 0) {
		mParentObj->mTransform->Position(parentPosition + forwardMove);
		mMoveInterval--;
	}
	else {
		if (mWaitInterval > 0) {
			mWaitInterval--;
		}
		else {
			mMoveInterval = mInitMoveInterval;
			mWaitInterval = mInitWaitInterval;
		}
	}
}