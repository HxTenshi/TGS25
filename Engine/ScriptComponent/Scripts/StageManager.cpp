#include "StageManager.h"
//#include "SailBoard.h"
//#include "Fade.h"
////アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"
#include "Input/Input.h"
#include "SceneCursor.h"
#include "Fade.h"
#include "RetryScene.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void StageManager::Initialize(){
	mButtonCreateCount = 0;
	mPauseCount = 0;
	mIsPause = false;
	mFadeOutObj = nullptr;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void StageManager::Start(){
	// リスタートの名前が空ならば、移動さきをタイトルにする
	if (mRetryScene == "") mRetryScene = "Title";
	// リスタートオブジェの生成
	auto retryObj = game->CreateActor("Assets/RetrySceneObj");
	game->AddObject(retryObj);
	retryObj->mTransform->SetParent(gameObject);
	auto retryScript = retryObj->GetScript<RetryScene>();
	// リスタートするシーンの名前を入れる
	retryScript->SetRetrySceneName(mRetryScene);
}

//毎フレーム呼ばれます
void StageManager::Update(){
	// キー入力
	if (Input::Trigger(PAD_DS4_KeyCoord::Button_OPTIONS) ||
		Input::Trigger(KeyCoord::Key_G)) {
		mPauseCount++;
		// ポーズの生成、削除
		if (mPauseCount % 2 == 1) createPause();
		else deletePause();
	}
	// ポーズでなければ返す
	if (!mIsPause) return;
	// 再開ボタンの場合はシーン遷移しない
	if (mCursorScript->IsPushCursor() &&
		mCursorScript->GetButtonCount() == 0) {
		mPauseCount = 0;
		deletePause();
		return;
	}
	// 特定のボタンが押されたらシーン遷移
	if (mCursorScript->IsChangeScene()) {
		// フェードアウトしてシーン移動
		mCursorScript->SetIsCursorMove(true);
		// 一度だけ生成
		if (mFadeOutObj == nullptr) {
			mFadeOutObj = game->CreateActor("Assets/Fade");
			game->AddObject(mFadeOutObj);
		}
		mFadeOutScript = mFadeOutObj->GetScript<Fade>();
		mFadeOutScript->FadeOut(mFadeOutSecond);
		// フェードアウト後シーン移動
		if (mFadeOutScript->IsFadeOut()) {
			game->DeltaTime()->SetTimeScale(1.0f);
			// シーン遷移ON
			mCursorScript->OnChangeScene();
		}
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void StageManager::Finish(){
}

//コライダーとのヒット時に呼ばれます
void StageManager::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void StageManager::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void StageManager::OnCollideExit(Actor* target){
	(void)target;
}

// ポーズ画面の生成
void StageManager::createPause() {
	// ボタンボックス
	auto poseButtons = game->CreateActor("Assets/SceneAssets/Buttons");
	game->AddObject(poseButtons);
	poseButtons->mTransform->SetParent(gameObject);
	// カーソル
	auto cursorObject = game->CreateActor("Assets/SceneAssets/SceneCursor");
	game->AddObject(cursorObject);
	cursorObject->mTransform->SetParent(gameObject);
	mCursorScript = cursorObject->GetScript<SceneCursor>();
	// ボタンの追加
	mCursorScript->AddButtonContainer("Continue_Button");
	// リトライ（未実装）
	mCursorScript->AddButtonContainer("Retry_Button");
	mCursorScript->AddButtonContainer("TitleBack_Button");
	// 遷移先のシーンの追加
	mCursorScript->AddSceneContainer("NotMove");
	// クレジット(未実装)
	mCursorScript->AddSceneContainer("RetryScene");
	mCursorScript->AddSceneContainer("Title");
	playPauseSE();
	game->DeltaTime()->SetTimeScale(0.0f);
	mIsPause = true;
}

// ポーズ画面の削除
void StageManager::deletePause() {
	game->DeltaTime()->SetTimeScale(1.0f);
	gameObject->mTransform->AllChildrenDestroy();
	playPauseSE();
	mIsPause = false;
}

// ポーズ画面切り替え時のサウンド再生
void StageManager::playPauseSE() {
	// ポーズ画面の切り替わり時に鳴らす
	auto sound = gameObject->GetComponent<SoundComponent>();
	if (!sound) return;
	sound->LoadFile("Assets/SceneAssets/SceneSound/Decision.wav");
	sound->Play();
}