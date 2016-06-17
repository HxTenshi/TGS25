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
	//// 初期の色を追加する
	//if (!mIsSetColor) {
	//	for (auto i = 0; i != mButtonContainer.size(); ++i) {
	//		auto mate = mButtonContainer[i]->GetComponent<MaterialComponent>();
	//		mInitColorContainer.push_back(mate->mAlbedo);
	//	}
	//	mIsSetColor = true;
	//}
	// 位置、大きさをボタンに合わせる
	auto buttonPosition = mButtonContainer[mButtonCount]->mTransform->Position();
	auto buttonScale = mButtonContainer[mButtonCount]->mTransform->Scale();
	gameObject->mTransform->Position(buttonPosition);
	gameObject->mTransform->Scale(buttonScale);
	//// 現在触れているボタンを暗くする
	//auto mate = mButtonContainer[mButtonCount]->GetComponent<MaterialComponent>();
	//auto color = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	//if (mate) mate->SetAlbedoColor(color);

	//auto begin = mButtonContainer.begin();
	if (Input::Trigger(PAD_DS4_KeyCoord::Button_UP) || Input::Trigger(KeyCoord::Key_A)) {
		if (mButtonCount > 0) {
			//if(mate) mate->SetAlbedoColor(mInitColorContainer[mButtonCount]);
			mButtonCount--;
		}
	}
	else if (Input::Trigger(PAD_DS4_KeyCoord::Button_DOWN) || Input::Trigger(KeyCoord::Key_Z)) {
		if (mButtonCount < mButtonContainer.size() - 1) {
			//if (mate) mate->SetAlbedoColor(mInitColorContainer[mButtonCount]);
			mButtonCount++;
		}
	}
	else if (Input::Trigger(PAD_DS4_KeyCoord::Button_CIRCLE) || Input::Trigger(KeyCoord::Key_SPACE)) {
		mIsChangeScene = true;
		/*auto sound = gameObject->GetComponent<SoundComponent>();
		if (!sound) return;
		sound->LoadFile("Assets/SceneTexture/Decision");
		sound->Play();*/
		game->Debug()->Log("他のシーン移動はまだしません");
	}
	//// アローのキーコードが反応しないです
	//// Key_DOWN == 0xD0 となっているので、DirectInputのキーコードとあっていても反応しないです
	//if (Input::Trigger(KeyCoord::Key_DOWN)) game->Debug()->Log("DownArrow");
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
	/*auto mate = buttonObj->GetComponent<MaterialComponent>();
	mInitColorContainer.push_back(mate->mAlbedo);*/
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