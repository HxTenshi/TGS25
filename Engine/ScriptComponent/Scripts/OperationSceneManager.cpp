#include "OperationSceneManager.h"
#include "Fade.h"
//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void OperationSceneManager::Initialize(){
	mIsChangeScene = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void OperationSceneManager::Start(){
	mFadeObj = game->CreateActor("Assets/Fade");
	game->AddObject(mFadeObj);
}

//毎フレーム呼ばれます
void OperationSceneManager::Update(){
	auto fadeInScript = mFadeObj->GetScript<Fade>();
	fadeInScript->FadeIn(mFadeSecond);
	if (!fadeInScript->IsFadeIn()) return;

	if (Input::Trigger(PAD_DS4_KeyCoord::Button_CROSS) ||
		Input::Trigger(PAD_X_KeyCoord::Button_A) ||
		Input::Trigger(KeyCoord::Key_H)) {
		mIsChangeScene = true;
		auto sound = gameObject->GetComponent<SoundComponent>();
		if (!sound) return;
		sound->LoadFile("Assets/SceneAssets/SceneSound/Decision.wav");
		sound->Play();
	}
	// ボタンが押されなければ戻らない
	if (!mIsChangeScene) return;
	// 一度だけ生成
	if (mFadeObj == nullptr) {
		mFadeObj = game->CreateActor("Assets/Fade");
		game->AddObject(mFadeObj);
	}
	auto fadeOutScript = mFadeObj->GetScript<Fade>();
	fadeOutScript->FadeOut(mFadeSecond);
	// フェードアウト後シーン移動
	if (fadeOutScript->IsFadeOut())
		game->LoadScene("./Assets/Scenes/Title.scene");
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void OperationSceneManager::Finish(){

}

//コライダーとのヒット時に呼ばれます
void OperationSceneManager::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void OperationSceneManager::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void OperationSceneManager::OnCollideExit(Actor* target){
	(void)target;
}