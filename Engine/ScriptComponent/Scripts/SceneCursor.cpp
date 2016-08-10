#include "SceneCursor.h"

//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"
#include "Engine\DebugEngine.h"

#include "Input/Input.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void SceneCursor::Initialize(){
	mButtonCount = 0;
	mPastButtonCount = 0;
	mLerpTime = 0.0f;
	mIsMove = false;
	mIsCursorMove = false;
	mIsPushCursor = false;
	mIsChangeScene = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void SceneCursor::Start(){
	auto buttonPosition = mButtonContainer[mButtonCount]->mTransform->Position();
	auto buttonScale = mButtonContainer[mButtonCount]->mTransform->Scale();
	auto scale = gameObject->mTransform->Scale();
	// ボタンの半分の大きさを取得する
	auto halfScale = XMVectorSet(
		buttonScale.x / 3.5f + scale.x / 2.0f + mAddCursorPositionX, 0.0f, 0.0f, 0.0f);
	// 位置の変更
	gameObject->mTransform->Position(buttonPosition - halfScale);
	if (mSetLerpTime <= 0) mSetLerpTime = 1;
}

//毎フレーム呼ばれます
void SceneCursor::Update(){
	// 一生動かない
	if (mIsMove) return;
	// キー操作で動く場合
	if (mIsCursorMove) {
		auto buttonPosition = mButtonContainer[mButtonCount]->mTransform->Position();
		auto buttonScale = mButtonContainer[mButtonCount]->mTransform->Scale();
		auto position = gameObject->mTransform->Position();
		auto scale = gameObject->mTransform->Scale();
		// ボタンの半分の大きさを取得する
		auto halfScale = XMVectorSet(
			buttonScale.x / 3.5f + scale.x / 2.0f + mAddCursorPositionX, 0.0f, 0.0f, 0.0f);
		// 線形補間ように
		if (mButtonCount != mPastButtonCount) {
			auto distance = buttonPosition - position;
			mLerp = (distance - halfScale) / mSetLerpTime;
			mPastButtonCount = mButtonCount;
		}
		// 位置の変更
		if (mLerpTime != 1.0f) {
			auto deltaTime = game->DeltaTime()->GetDeltaTime();
			auto timeScale = game->DeltaTime()->GetTimeScale();
			auto moveVelocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			if (deltaTime == 0.0f) moveVelocity = mLerp * 0.0167f * mSpeed;
			else moveVelocity = mLerp * deltaTime * mSpeed;
			auto distance = buttonPosition - position;
			if (abs(moveVelocity.x) > abs(distance.x) ||
				abs(moveVelocity.y) > abs(distance.y))
				gameObject->mTransform->Position(buttonPosition - halfScale);
			else gameObject->mTransform->Position(position + moveVelocity);
			//mLerpTime += 1.0f / mSetLerpTime * deltaTime * mSpeed;
			auto lerpSpeed = 0.0f;
			if (deltaTime == 0.0f) lerpSpeed = 1.0f / mSetLerpTime * 0.0167f * mSpeed;
			else lerpSpeed = 1.0f / mSetLerpTime * deltaTime * mSpeed;
			mLerpTime += lerpSpeed;
			if (mLerpTime >= 1.0f) mLerpTime = 1.0f;
		}
		else{
			gameObject->mTransform->Position(buttonPosition - halfScale);
			mLerpTime = 0.0f;
			mIsCursorMove = false;
		}
	}
	// カーソルが動き終わるまで入力処理を行わない
	if (mIsCursorMove) return;
	// シーンが変わるなら入力処理を行わない
	if (mIsChangeScene) return;
	// キー入力
	if (Input::Trigger(PAD_DS4_KeyCoord::Button_UP) ||
		Input::Trigger(KeyCoord::Key_A)) {
		if (mButtonCount == 0) return;
		mButtonCount--;
		mIsCursorMove = true;
	}
	else if (Input::Trigger(PAD_DS4_KeyCoord::Button_DOWN) ||
		Input::Trigger(KeyCoord::Key_Z)) {
		if (mButtonCount == mButtonContainer.size() - 1) return;
		mButtonCount++;
		mIsCursorMove = true;
	}
	else if (Input::Trigger(PAD_DS4_KeyCoord::Button_CIRCLE) ||
		Input::Trigger(KeyCoord::Key_SPACE)) {
		mIsPushCursor = true;
		// シーン遷移しない場合はすぐに返す
		if (mSceneContainer[mButtonCount] == "./Assets/Scenes/NotMove.scene") {
			return;
		}
		auto sound = gameObject->GetComponent<SoundComponent>();
		if (!sound) return;
		sound->LoadFile("Assets/SceneAssets/SceneSound/Decision.wav");
		sound->Play();
		// シーンの遷移 true
		mIsCursorMove = true;
		mIsChangeScene = true;
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void SceneCursor::Finish(){
}

//コライダーとのヒット時に呼ばれます
void SceneCursor::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void SceneCursor::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void SceneCursor::OnCollideExit(Actor* target){
	(void)target;
}

// ボタンの配列に追加します
void SceneCursor::AddButtonContainer(const char* buttonName) {
	mButtonContainer.push_back(game->FindActor(buttonName));
}

// シーンの配列に追加します
void SceneCursor::AddSceneContainer(const std::string sceneName) {
	std::string addSceneName = "./Assets/Scenes/" + sceneName + ".scene";
	mSceneContainer.push_back(addSceneName);
}

// 押されたボタンによってシーンを移動します
void SceneCursor::OnChangeScene() {
	game->LoadScene(mSceneContainer[mButtonCount]);
}

// カーソルの移動制限の設定します
void SceneCursor::SetIsCursorMove(bool isMove) {
	mIsMove = isMove;
}

// ボタンの値を取得します
int SceneCursor::GetButtonCount() {
	return mButtonCount;
}

// カーソルが動いているかを返します
bool SceneCursor::IsCursorMove() {
	return mIsCursorMove;
}

// カーソルが押されたかを返します
bool SceneCursor::IsPushCursor() {
	return mIsPushCursor;
}

// シーンが変わったかを返します
bool SceneCursor::IsChangeScene() {
	return mIsChangeScene;
}