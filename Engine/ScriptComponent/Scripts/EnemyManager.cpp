#include "EnemyManager.h"
//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"
#include "Engine\DebugEngine.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void EnemyManager::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void EnemyManager::Start(){
	// 親である敵の取得
	mEnemyObj = gameObject->mTransform->GetParent();
	mEnemyParentObj = mEnemyObj->mTransform->GetParent();
	mInitParentPosition = mEnemyParentObj->mTransform->Position();
	mInitParentRotate = mEnemyParentObj->mTransform->Rotate();
}

//毎フレーム呼ばれます
void EnemyManager::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void EnemyManager::Finish(){

}

//コライダーとのヒット時に呼ばれます
void EnemyManager::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void EnemyManager::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void EnemyManager::OnCollideExit(Actor* target){
	(void)target;
}

// 敵の目の前にオブジェを生成します(敵の親に置く場合)
void EnemyManager::SetParentForwardObj(Actor* setObj) {
	//auto enemyParent = mEnemyObj->mTransform->GetParent();
	auto parentPosition = mEnemyParentObj->mTransform->Position();
	auto parentRotate = mEnemyParentObj->mTransform->Rotate();
	auto collider = mEnemyObj->GetComponent<PhysXColliderComponent>();
	auto colliderScale = collider->GetScale();
	auto setObjScale = setObj->mTransform->Scale();
	// 位置の指定
	auto setPosition = XMVectorSet(
		((colliderScale.z / 2.0f) + (setObjScale.z / 2.0f)) * sinf(parentRotate.y),
		0.0f,
		((colliderScale.z / 2.0f) + (setObjScale.z / 2.0f)) * cosf(parentRotate.y), 0.0f);
	// 位置の変更
	setObj->mTransform->Position(parentPosition + -setPosition);
	// 生成元のオブジェの回転角に変更
	setObj->mTransform->Rotate(parentRotate);
}

// 親のステータスの初期化をします
void EnemyManager::InitStatus() {
	mEnemyParentObj->mTransform->Position(mInitParentPosition);
	mEnemyParentObj->mTransform->Rotate(mInitParentRotate);
}

// 敵のステータスのリセットします
void EnemyManager::ResetStatus() {
	auto ZeroStatus = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	// ０の値に変える
	mEnemyObj->mTransform->Position(ZeroStatus);
	mEnemyObj->mTransform->Rotate(ZeroStatus);
}

// サウンドを再生します
void EnemyManager::EnemyPlaySound(const std::string soundName) {
	std::string playSoundName = "Assets/Enemy/" + soundName + ".wav";
	// サウンドを鳴らす
	auto sound = gameObject->GetComponent<SoundComponent>();
	if (!sound) return;
	sound->LoadFile(playSoundName);
	sound->Play();
}

// プレイヤーと指定されたオブジェの位置との距離を計算して返します
float EnemyManager::GetPlayerDistance(Actor* playerObj, Actor* otherObj) {
	auto playerPosition = playerObj->mTransform->Position();
	auto otherPosition = otherObj->mTransform->Position();
	// プレイヤーとの距離を計算
	auto distance = XMVector3Length(playerPosition - otherPosition);
	return distance.z;
}

// デルタタイムを取得します
float EnemyManager::GetEnemyDeltaTime(float framerate) {
	auto deltaTime = game->DeltaTime()->GetDeltaTime();
	return deltaTime * framerate;
}