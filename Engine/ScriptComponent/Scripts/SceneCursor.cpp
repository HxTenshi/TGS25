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
	mIsChangeScene = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void SceneCursor::Start(){
	// 各カーソルの捜索
	mCursorRightTop = game->FindActor("Cursor_Right_Top");
	mCursorRightBottom = game->FindActor("Cursor_Right_Bottom");
	mCursorLeftTop = game->FindActor("Cursor_Left_Top");
	mCursorLeftBottom = game->FindActor("Cursor_Left_Bottom");
	// カーソルコンテナに追加
	mCursorContainer.push_back(mCursorRightTop);
	mCursorContainer.push_back(mCursorRightBottom);
	mCursorContainer.push_back(mCursorLeftTop);
	mCursorContainer.push_back(mCursorLeftBottom);
	// カーソル座標コンテナに追加
	mCursorVelocityContainer.push_back(XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f));
	mCursorVelocityContainer.push_back(XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f));
	mCursorVelocityContainer.push_back(XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f));
	mCursorVelocityContainer.push_back(XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f));
}

//毎フレーム呼ばれます
void SceneCursor::Update(){
	// シーンが変わるなら入力処理を行わない
	if (mIsChangeScene) return;
	// カーソルの位置と大きさの変更
	for (auto i = 0; i != 4; i++) {
		auto cursor = mCursorContainer[i];
		// 位置をボタンに合わせる
		auto buttonPosition = mButtonContainer[mButtonCount]->mTransform->Position();
		auto buttonScale = mButtonContainer[mButtonCount]->mTransform->Scale();
		// ボタンの半分の大きさを取得する
		auto halfScale = XMVectorSet(
			buttonScale.x / 2.0f + mAddCursorPositionX,
			buttonScale.y / 2.0f + mAddCursorPositionY, 0.0f, 0.0f);
		// 位置の変更
		cursor->mTransform->Position(
			buttonPosition + (halfScale * mCursorVelocityContainer[i]));
		// 大きさの変更
		auto cursorScale = XMVectorSet(mCursorScaleX, mCursorScaleY, 0.0f, 0.0f);
		cursor->mTransform->Scale(cursorScale);
	}
	// キー入力
	if (Input::Trigger(PAD_DS4_KeyCoord::Button_UP) || Input::Trigger(KeyCoord::Key_A)) {
		if (mButtonCount > 0) {
			mButtonCount--;
		}
	}
	else if (Input::Trigger(PAD_DS4_KeyCoord::Button_DOWN) || Input::Trigger(KeyCoord::Key_Z)) {
		if (mButtonCount < mButtonContainer.size() - 1) {
			mButtonCount++;
		}
	}
	else if (Input::Trigger(PAD_DS4_KeyCoord::Button_CIRCLE) || Input::Trigger(KeyCoord::Key_SPACE)) {
		mIsChangeScene = true;
		auto sound = gameObject->GetComponent<SoundComponent>();
		if (!sound) return;
		sound->LoadFile("Assets/SceneTexture/Decision.wav");
		sound->Play();
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

// ボタンの値を取得します
int SceneCursor::GetButtonCount() {
	return mButtonCount;
}

// シーンが変わったかを返します
bool SceneCursor::IsChangeScene() {
	return mIsChangeScene;
}