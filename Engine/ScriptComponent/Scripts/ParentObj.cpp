#include "ParentObj.h"
#include "Enemy.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Engine\DebugEngine.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void ParentObj::Initialize(){
	/*auto childrenObj = gameObject->mTransform->Children();
	mEnemyScript = childrenObj.*/

	//mCreateCount = gameObject->mTransform->Children().size();
	mCreateCount = 0;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ParentObj::Start(){
	// 親がいた場合は位置を調整する
	if (gameObject->mTransform->GetParent() != nullptr) {
		auto initPosi = XMVectorSet(mPositionX, 0.0f, mPositionZ, 0.0f);
		gameObject->mTransform->Position(initPosi);
	}
}

//毎フレーム呼ばれます
void ParentObj::Update(){
	if (mCreateCount == 0) {
		//baseStr = "Assets/";
		//std::string createStr = baseStr + gameObject->Name();
		//createStr.erase(createStr.end() - 9, createStr.end());
		//// string型をchar*に変換
		//createObjName = createStr.c_str();
		//game->Debug()->Log(createObjName);

		// 子の生成 自分の名前から子を選択し生成します
		std::string baseName = "Assets/Enemy/EnemyObj/" + gameObject->Name();
		baseName.erase(baseName.end() - 9, baseName.end());
		auto createObjName = baseName.c_str();

		// auto parentObj = game->CreateActor("Assets/FlyingFish");
		auto enemyObj = game->CreateActor(createObjName);
		game->AddObject(enemyObj);
		enemyObj->mTransform->SetParent(gameObject);

		/*mEnemyScriptName = gameObject->Name();
		mEnemyScript = enemyObj->GetScript<Enemy>();*/

		mCreateCount = 1;
	}

	/*if (!(mEnemyScript->IsDead())) {
		game->Debug()->Log("消去");
		game->DestroyObject(gameObject);
	}*/
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ParentObj::Finish(){

}

//コライダーとのヒット時に呼ばれます
void ParentObj::OnCollideBegin(Actor* target){
	//Enemy::OnCollideBegin(target);
	mEnemyScript->OnCollideBegin(target);
}

//コライダーとのヒット中に呼ばれます
void ParentObj::OnCollideEnter(Actor* target){
	//(void)target;
	mEnemyScript->OnCollideEnter(target);
}

//コライダーとのロスト時に呼ばれます
void ParentObj::OnCollideExit(Actor* target){
	//(void)target;
	mEnemyScript->OnCollideExit(target);
}

void ParentObj::SetPosition(float x, float z) {
	mPositionX = x;
	mPositionZ = z;
}
