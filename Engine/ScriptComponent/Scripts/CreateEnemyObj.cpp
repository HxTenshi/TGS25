#include "CreateEnemyObj.h"
#include "ParentObj.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Engine\DebugEngine.h"
// 乱数時間の使用
#include <time.h>


//生成時に呼ばれます（エディター中も呼ばれます）
void CreateEnemyObj::Initialize(){
	mIsCreateObj = false;
	mBaseStr = "Assets/Enemy/EnemyParentObj/";
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void CreateEnemyObj::Start(){
	mInitCreateTimeInterval = mCreateTimeInterval;
	mInitCreateDelayTime = mCreateDelayTime;
	// 名前コンテナに追加
	mNameContainer.push_back("FlyingFish");
	mNameContainer.push_back("KillerWhale");

	// 現在の時刻で乱数を行うように設定
	srand((unsigned int) time(NULL));
}

//毎フレーム呼ばれます
void CreateEnemyObj::Update(){

	auto playerObj = game->FindActor("Board");
	auto playerPosition = playerObj->mTransform->Position();
	auto playerRotate = playerObj->mTransform->Rotate();
	auto thisPosition = gameObject->mTransform->Position();
	auto distance = XMVector3Length(playerPosition - thisPosition);
	// 無限生成
	if (mIsInfinityCreate) mCreateCount = 100;
	// 生成
	if (distance.z <= mReactionDistance && mCreateCount > 0) {
		// 生成間隔が０で、生成オブジェクトがある場合生成する
		if (mCreateTimeInterval <= 0) {

			if (!mIsCreateObj) {
				// 複数生成
				for (auto i = 0; i != mOnceCreateCount; i++) {
					CreateName();
					// 敵オブジェの生成
					auto createObj = game->CreateActor(mCreateObjName);
					// 生成元がなかったら生成しない
					//if (createObj == nullptr) return;
					game->AddObject(createObj);
					// 生成オブジェの子供に設定(終了時に削除などをするため)
					createObj->mTransform->SetParent(gameObject);
					// 子供に追加するので値の加算を行わない
					// ランダムで位置を代入する
					auto enemyPosition = XMVectorSet(
						GetRandom(-mRandomCreateRange, mRandomCreateRange) / 10.0f,
						GetRandom(-5, 20) / 10.0f,
						GetRandom(-mRandomCreateRange, mRandomCreateRange) / 10.0f,
						0.0f);
					// 位置の変更
					createObj->mTransform->Position(enemyPosition);
				}
				game->Debug()->Log("生成");
				// 生成カウントの減算
				mCreateCount--;
				mIsCreateObj = true;
			}
			// ディレイタイムの減算
			mCreateDelayTime--;
			if (mCreateDelayTime <= 0) {
				// 各パラメータの初期化
				mCreateTimeInterval = mInitCreateTimeInterval;
				mCreateDelayTime = mInitCreateDelayTime;
				mIsCreateObj = false;
			}			
		}
		else {
			// 生成時間の減算
			mCreateTimeInterval--;
		}
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void CreateEnemyObj::Finish(){
	gameObject->mTransform->AllChildrenDestroy();
}

//コライダーとのヒット時に呼ばれます
void CreateEnemyObj::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void CreateEnemyObj::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void CreateEnemyObj::OnCollideExit(Actor* target){
	(void)target;
}

// 生成オブジェクトの名前を設定します
void CreateEnemyObj::CreateName() {
	// 生成オブジェの名前指定
	std::string createStr;
	if (!mIsRandom) {
		// このスクリプトを持つオブジェの名前から判断します
		if(mEnemyName == "") createStr = mBaseStr + gameObject->Name();
		else createStr = mBaseStr + mEnemyName;
		createStr.erase(createStr.end() - 9, createStr.end());
	}	// ランダム
	else createStr = mBaseStr + mNameContainer[GetRandom(0, 1)];
	createStr = createStr + "ParentObj";
	// string型をchar*に変換
	mCreateObjName = createStr.c_str();
}

// ランダム関数です
int CreateEnemyObj::GetRandom(int min, int max) {
	// 範囲乱数公式…らしいです
	return min + (int)(rand() * (max - min + 1.0f) / (1.0f + RAND_MAX));
}

// 敵が生成オブジェクトに戻る距離を返します
float CreateEnemyObj::GetReturnDistance() {
	return mEnemyReturnDistance;
}