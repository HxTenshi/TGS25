#include "ParentObj.h"
#include "Enemy.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Engine\DebugEngine.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void ParentObj::Initialize(){
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ParentObj::Start(){
	// 子の生成 自分の名前から子を選択し生成します
	std::string baseName = "Assets/Enemy/EnemyObj/" + gameObject->Name();
	baseName.erase(baseName.end() - 9, baseName.end());
	auto createObjName = baseName.c_str();
	// 子である敵の生成
	auto enemyObj = game->CreateActor(createObjName);
	game->AddObject(enemyObj);
	enemyObj->mTransform->SetParent(gameObject);
}

//毎フレーム呼ばれます
void ParentObj::Update(){
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ParentObj::Finish(){
}

//コライダーとのヒット時に呼ばれます
void ParentObj::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void ParentObj::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void ParentObj::OnCollideExit(Actor* target){
	(void)target;
}

void ParentObj::SetPosition(float x, float z) {
	mPositionX = x;
	mPositionZ = z;
}
