#include "RetryScene.h"
//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"

// 静的メンバ変数の実体
std::string RetryScene::mRetrySceneName = "";

//生成時に呼ばれます（エディター中も呼ばれます）
void RetryScene::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void RetryScene::Start(){

}

//毎フレーム呼ばれます
void RetryScene::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void RetryScene::Finish(){

}

//コライダーとのヒット時に呼ばれます
void RetryScene::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void RetryScene::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void RetryScene::OnCollideExit(Actor* target){
	(void)target;
}

// リスタートシーンの名前を入れます
void RetryScene::SetRetrySceneName(const std::string name) {
	mRetrySceneName = "./Assets/Scenes/" + name + ".scene";
}

// リスタートシーンの名前を取得します
std::string RetryScene::GetRetrySceneName() {
	return mRetrySceneName;
}