#include "CreateEnemyObj.h"
#include "ParentObj.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Engine\DebugEngine.h"
// 乱数の使用
//#include <random>
#include <time.h>


//生成時に呼ばれます（エディター中も呼ばれます）
void CreateEnemyObj::Initialize(){
	mIsCreateObj = false;

	/*auto createName = ("Assets/KillerWhale");
	game->Debug()->Log(createName);*/
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void CreateEnemyObj::Start(){
	mInitCreateTimeInterval = mCreateTimeInterval;
	mInitCreateDelayTime = mCreateDelayTime;

	mVectorCount = 0;
	mRandomCreateRange = 20;

	// 現在の時刻で乱数を行うように設定
	srand((unsigned int) time(NULL));

	//game->Debug()->Log(createObjName);

	//mCreateObj = game->CreateActor(createObjName);
	//if (mCreateObj == NULL) {
	//	//mCreateObj = game->CreateActor("Assets/FlyingFish");
	//}
	//game->AddObject(mCreateObj);
}

//毎フレーム呼ばれます
void CreateEnemyObj::Update(){

	auto playerObj = game->FindActor("Board");
	auto playerPosition = playerObj->mTransform->Position();
	auto playerRotate = playerObj->mTransform->Rotate();
	auto thisPosition = gameObject->mTransform->Position();
	auto distance = XMVector3Length(playerPosition - thisPosition);

	if (distance.z <= mReactionDistance && mCreateCount > 0) {

		if (mCreateTimeInterval <= 0) {

			if (!mIsCreateObj) {
				// 敵オブジェの生成
				//mCreateObj = game->CreateActor(createObjName);
				////if (mCreateObj == NULL) {
				////	//mCreateObj = game->CreateActor("Assets/FlyingFish");
				////}
				//game->AddObject(mCreateObj);

				/*std::random_device random;
				std::mt19937 mt(random);

				std::uniform_int_distribution<int> rand(0, 200);

				game->Debug()->Log(std::to_string(rand(mt)));*/
				game->Debug()->Log(std::to_string(GetRandom(-mRandomCreateRange, mRandomCreateRange)));
				game->Debug()->Log(std::to_string(GetRandom(-mRandomCreateRange, mRandomCreateRange)));

				// 生成オブジェの名前指定
				// このスクリプトを持つオブジェの名前から判断します
				baseStr = "Assets/Enemy/EnemyParentObj/";
				std::string createStr = baseStr + gameObject->Name();
				createStr.erase(createStr.end() - 9, createStr.end());
				createStr = createStr + "ParentObj";
				// string型をchar*に変換
				createObjName = createStr.c_str();

				auto createObj = game->CreateActor(createObjName);
				// 生成オブジェの子供に設定(終了時に削除するため)
				//createObj->mTransform->SetParent(gameObject);

				game->AddObject(createObj);
				createObj->mTransform->Position(gameObject->mTransform->Position());
				//auto thisPositoin = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				//createObj->mTransform->Position(thisPosition);

				
				mCreateCount--;

				mIsCreateObj = true;
			}
			
			mCreateDelayTime--;
			
			if (mCreateDelayTime <= 0) {
				// 各パラメータの初期化
				mCreateTimeInterval = mInitCreateTimeInterval;
				mCreateDelayTime = mInitCreateDelayTime;
				mIsCreateObj = false;
			}			
		}
		else {
			mCreateTimeInterval--;
		}
	
	}

	if (mCreateCount <= 0) {
		auto material = gameObject->GetComponent<MaterialComponent>();
		auto color = XMFLOAT4(0, 0, 1, 1);

		material->SetAlbedoColor(color);
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void CreateEnemyObj::Finish(){
	//gameObject->mTransform->Children().clear();
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


int CreateEnemyObj::GetRandom(int min, int max) {
	// 範囲乱数公式…らしいです
	return min + (int)(rand() * (max - min + 1.0f) / (1.0f + RAND_MAX));
}