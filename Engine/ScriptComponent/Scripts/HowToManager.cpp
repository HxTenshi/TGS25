#include "HowToManager.h"
//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"
#include "Input/Input.h"
#include "Game\Component\MovieComponent.h"
#include "Fade.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void HowToManager::Initialize(){
	//mMovieTimer = 0.0f;
	//mMovieEndTime = 0.0f;
	//mMovieEndTime = 47.5f * 29.0f;
	mIsChangeScene = false;
	mIsFadeIn = false;
	mIsPlay = false;
	mName = "";
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void HowToManager::Start(){


}

//毎フレーム呼ばれます
void HowToManager::Update(){
	if (!mIsFadeIn) {
		mFadeOutObj = game->FindActor("Fade");
		if (mFadeOutObj == nullptr) mIsFadeIn = true;
		auto fadeInScript = mFadeOutObj->GetScript<Fade>();
		fadeInScript->FadeIn(mFadeOutSecond);
		if (fadeInScript->IsFadeIn()) {
			game->DestroyObject(mFadeOutObj);
			mFadeOutObj = nullptr;
			mIsFadeIn = true;
		}
	}
	else {
		// ムービー再生
		auto movie = gameObject->GetComponent<MovieComponent>();
		if (!mIsPlay){
			movie->LoadFile(mName);
			movie->SetLoop(false);
			movie->Play();
			mIsPlay = true;
		}
		else if (!movie->IsPlay()) {
			movie->Stop();
			mIsChangeScene = true;
		}
		//if (!movie->IsPlay()) mIsChangeScene = true;
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
			if (mFadeOutScript->IsFadeOut()) {
				movie->Stop();
				game->LoadScene("Assets/Scenes/Title.scene");
			}
		}
		// ボタンが押されたら二度と押せない
		if (mIsChangeScene) return;
		// ボタンが押されたら強制終了？
		if (Input::Trigger(PAD_DS4_KeyCoord::Button_CIRCLE) || Input::Trigger(KeyCoord::Key_SPACE)) {
			mIsChangeScene = true;
			auto sound = gameObject->GetComponent<SoundComponent>();
			if (!sound) return;
			sound->LoadFile("Assets/SceneAssets/SceneSound/Decision.wav");
			sound->Play();
		}
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

// 再生するムービーを変更します
void HowToManager::SetMovie(std::string name, float time) {
	mName = "Assets/SceneAssets/SceneMovie/" + name + ".wmv";
	//mMovieEndTime = time;
	// mMovieEndTime = 47.5f * 29.0f;
}