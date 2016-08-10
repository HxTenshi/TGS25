#include "RetrySceneManager.h"
//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"

#include "RetryScene.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void RetrySceneManager::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void RetrySceneManager::Start(){
	// リスタートオブジェの生成
	auto retryObj = game->CreateActor("Assets/RetrySceneObj");
	game->AddObject(retryObj);
	auto retryScript = retryObj->GetScript<RetryScene>();
	mSceneName = retryScript->GetRetrySceneName();
}

//毎フレーム呼ばれます
void RetrySceneManager::Update(){
	// シーン移動
	if (mSceneName == "") 
		mSceneName = "./Assets/Scenes/Title.scene";
	game->LoadScene(mSceneName);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void RetrySceneManager::Finish(){

}

//コライダーとのヒット時に呼ばれます
void RetrySceneManager::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void RetrySceneManager::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void RetrySceneManager::OnCollideExit(Actor* target){
	(void)target;
}