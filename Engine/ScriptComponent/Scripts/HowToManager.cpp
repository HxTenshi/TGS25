#include "HowToManager.h"
//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"
#include "Input/Input.h"
#include "Fade.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void HowToManager::Initialize(){
	mIsChangeScene = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void HowToManager::Start(){

}

//毎フレーム呼ばれます
void HowToManager::Update(){
	// ムービー再生
	// 再生完了がしたらタイトルシーンに遷移
	// 一度だけ生成
	if (mIsChangeScene) {
		if (mFadeOutObj == nullptr) {
			mFadeOutObj = game->CreateActor("Assets/Fade");
			game->AddObject(mFadeOutObj);
		}
		auto mFadeOutScript = mFadeOutObj->GetScript<Fade>();
		mFadeOutScript->FadeOut(mFadeOutSecond);
		// フェードアウト後シーン移動
		if (mFadeOutScript->IsFadeOut()) game->LoadScene("Assets/Scenes/Title.scene");
	}
	// ボタンが押されたら二度と押せない
	if (mIsChangeScene) return;
	// ボタンが押されたら強制終了？
	if (Input::Trigger(PAD_DS4_KeyCoord::Button_CIRCLE) || Input::Trigger(KeyCoord::Key_SPACE)) {
		mIsChangeScene = true;
		auto sound = gameObject->GetComponent<SoundComponent>();
		if (!sound) return;
		sound->LoadFile("Assets/SceneTexture/Decision.wav");
		sound->Play();
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void HowToManager::Finish(){

}

//コライダーとのヒット時に呼ばれます
void HowToManager::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void HowToManager::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void HowToManager::OnCollideExit(Actor* target){
	(void)target;
}