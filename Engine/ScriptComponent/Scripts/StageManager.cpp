#include "StageManager.h"
//#include "SailBoard.h"
//#include "Fade.h"
////アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"
#include "Input/Input.h"
#include "PlayerManager.h"
#include "SceneCursor.h"
#include "Fade.h"
#include "RetryScene.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void StageManager::Initialize(){
	mSetParentCount = 0;
	mButtonCreateCount = 0;
	mPauseCount = 0;
	mChangeTime = 2.0f * 60.0f;
	mSceneChangeTimer = 0.0f;
	mIsPause = false;
	mFadeOutObj = nullptr;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void StageManager::Start(){
	
}

//毎フレーム呼ばれます
void StageManager::Update(){
	if (mSetParentCount == 0) {
		// プレイヤーマネージャに持たせる
		auto playerManagerObj = game->FindActor("PlayerManager");
		gameObject->mTransform->SetParent(playerManagerObj);
		mSetParentCount = 1;
	}
	// プレイヤーを探す
	auto playerManager = game->FindActor("PlayerManager");
	if (playerManager->Name() != "PlayerManager") return;
	auto playerManagerScript = playerManager->GetScript<PlayerManager>();
	// フェードオブジェクトを探す
	auto fadeObj = game->FindActor("Fade");
	auto startObj = game->FindActor("Start");
	// ゲーム開始状態　もしくは　フェードオブジェクトがある場合ならばすぐに返す
	if (!playerManagerScript->IsGameStart()) return;
	// キー入力
	if (fadeObj == nullptr && startObj == nullptr) {
		if (Input::Trigger(PAD_DS4_KeyCoord::Button_OPTIONS) ||
			Input::Trigger(PAD_X_KeyCoord::Button_START) ||
			Input::Trigger(KeyCoord::Key_G)) {
			mPauseCount++;
			// ポーズの生成、削除
			if (mPauseCount % 2 == 1) createPause();
			else deletePause();
		}
		else if (
			(Input::Trigger(PAD_DS4_KeyCoord::Button_CROSS) ||
				Input::Trigger(PAD_X_KeyCoord::Button_A) ||
				Input::Trigger(KeyCoord::Key_H)) &&
			mPauseCount % 2 == 1) {
			mPauseCount++;
			deletePause();
		}
		// ×ボタンが押されてもポーズ画面を消すようにする
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
	// タイムが一定以上になったらフェイドアウトしてPVシーンに移行
	if (mChangeTime > mSceneChangeTimer) {
		if (mCursorScript->IsCursorMove()) mSceneChangeTimer = 0.0f;
		if (!mCursorScript->IsChangeScene())mSceneChangeTimer += 0.0167f;
	}
	// 特定のボタンが押されたらシーン遷移
	if (mCursorScript->IsChangeScene() ||
		mChangeTime <= mSceneChangeTimer) {
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
			if (mChangeTime <= mSceneChangeTimer) game->LoadScene("./Assets/Scenes/Title.scene");
			else {
				// リスタートの名前が空ならば、移動さきをタイトルにする
				if (mRetryScene == "") mRetryScene = "Title";
				if (mCursorScript->GetButtonCount() == 2) mRetryScene = "Title";
				// リスタートオブジェの生成
				auto retryObj = game->CreateActor("Assets/RetrySceneObj");
				game->AddObject(retryObj);
				retryObj->mTransform->SetParent(gameObject);
				auto retryScript = retryObj->GetScript<RetryScene>();
				// リスタートするシーンの名前を入れる
				retryScript->SetRetrySceneName(mRetryScene);
				mCursorScript->OnChangeScene();
			}
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
	// フェードの作成
	auto fadeObj = game->CreateActor("Assets/Fade");
	game->AddObject(fadeObj);
	fadeObj->mTransform->SetParent(gameObject);
	fadeObj->Name("PauseFade");
	auto fadeScript = fadeObj->GetScript<Fade>();
	fadeScript->SetFadeAlpha(mFadeAlpha);
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
	mCursorScript->AddSceneContainer("RetryScene");
	playPauseSE();
	game->DeltaTime()->SetTimeScale(0.0f);
	mSceneChangeTimer = 0.0f;
	mIsPause = true;
}

// ポーズ画面の削除
void StageManager::deletePause() {
	auto fadeObj = game->FindActor("PauseFade");
	if (fadeObj != nullptr) game->DestroyObject(fadeObj);
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