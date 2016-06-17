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

}

//毎フレーム呼ばれます
void SceneCursor::Update(){
	// シーンが変わるなら入力しない
	if (mIsChangeScene) return;
	// 位置、大きさをボタンに合わせる
	auto buttonPosition = mButtonContainer[mButtonCount]->mTransform->Position();
	auto buttonScale = mButtonContainer[mButtonCount]->mTransform->Scale();
	gameObject->mTransform->Position(buttonPosition);
	gameObject->mTransform->Scale(buttonScale);

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

// 
int SceneCursor::GetButtonCount() {
	return mButtonCount;
}

// 
bool SceneCursor::IsChangeScene() {
	return mIsChangeScene;
}