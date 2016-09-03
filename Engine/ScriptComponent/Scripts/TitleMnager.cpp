#include "TitleMnager.h"
#include "Input/Input.h"
//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"PhysX\IPhysXEngine.h"
// カーソルとフェード追加
#include "SceneCursor.h"
#include "Fade.h"
#include "HowToManager.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void TitleMnager::Initialize(){
	mTitleButtonCount = 0;
	mSceneChangeTimer = 0.0f;
	// タイトルセレクト
	mSelectNumberContainer.push_back("StageSelectButtons");
	mSelectNumberContainer.push_back("Assets/SceneAssets/Title_Buttons");
	mSelectNumberContainer.push_back("GameStart_Button");
	mSelectNumberContainer.push_back("How_To_Button");
	mSelectNumberContainer.push_back("GameEnd_Button");
	mSelectNumberContainer.push_back("NotMove");
	mSelectNumberContainer.push_back("How_To");
	mSelectNumberContainer.push_back("GameEnd");
	// ステージセレクト
	mSelectNumberContainer.push_back("Title_Buttons");
	mSelectNumberContainer.push_back("Assets/SceneAssets/StageSelectButtons");
	mSelectNumberContainer.push_back("Stage01_Button");
	mSelectNumberContainer.push_back("Stage02_Button");
	mSelectNumberContainer.push_back("Stage03_Button");
	mSelectNumberContainer.push_back("FastStage");
	mSelectNumberContainer.push_back("Stage01");
	mSelectNumberContainer.push_back("Stage02");
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void TitleMnager::Start(){
	// カーソルの取得
	mCursorObj = game->FindActor("SceneCursor");
	mCursorScript = mCursorObj->GetScript<SceneCursor>();
	// ボタンの追加
	createButton(TITLESELECT_NUMBER, TITLESELECT_COUNT);
}

//毎フレーム呼ばれます
void TitleMnager::Update()
{
	auto center = game->FindActor("CenterPosition");
	auto camera = game->FindActor("MainCameraParent");
	auto cameraPosition = camera->mTransform->Position();
	auto cameraRotate = camera->mTransform->Rotate();
	// デルタタイムの取得
	auto deltaTime = game->DeltaTime()->GetDeltaTime();

	if (mTitleButtonCount % 2 == 1) {
		if (mCursorScript->IsPushCursor() &&
			mCursorScript->GetButtonCount() == 0)
			createButton(STAGESELECT_NUMBER, STAGESELECT_COUNT);
	}
	else {
		if (mCursorScript->IsBackCursor())
			createButton(TITLESELECT_NUMBER, TITLESELECT_COUNT);
	}
	// createButton()

	// 何も入力がなければタイム加算
	// タイムが一定以上になったらフェイドアウトしてPVシーンに移行
	if (mChangeTime > mSceneChangeTimer) {
		if (mCursorScript->IsCursorMove()) mSceneChangeTimer = 0.0f;
		if (!mCursorScript->IsChangeScene())mSceneChangeTimer += deltaTime;
	}
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
			if (mChangeTime <= mSceneChangeTimer) game->LoadScene("./Assets/Scenes/PVPlay.scene");
			else  mCursorScript->OnChangeScene();
		}
	}
	// カメラの親の移動
	camera->mTransform->Position(
		cameraPosition +
		(camera->mTransform->Left() * (mCameraRotateSpeed * 0.01f) * (deltaTime * 60.0f)));
	// + (camera->mTransform->Left() * (mCameraRotateSpeed * (3.14f * 2.0f)) * 0.01)
	//カメラの親を回転させる -> カメラ本体を回転可能にさせるため 
	auto centerPosition = center->mTransform->Position();
	auto v = centerPosition - camera->mTransform->Position();
	auto angle = atan2(v.x, v.z);
	auto escapeAngle = angle;
	if (std::to_string(escapeAngle) == "0.000000") escapeAngle = 0.0f;
	if (escapeAngle < 0) escapeAngle += 3.141593f * 2.0f;

	camera->mTransform->Rotate(camera->mTransform->Up() * (escapeAngle));
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void TitleMnager::Finish(){
	game->DestroyObject(mFadeOutObj);
}

//コライダーとのヒット時に呼ばれます
void TitleMnager::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void TitleMnager::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void TitleMnager::OnCollideExit(Actor* target){
	(void)target;
}

// タイトルのデフォルトのボタンを作成します
void TitleMnager::createButton(const int number, const int count) {
	//mSelectNumberContainer[selectNumber]
	/*auto stageSelectButtons = game->FindActor("StageSelectButtons");
	if (stageSelectButtons != nullptr) {
		deleteButtonObj("StageSelectButtons");
		auto titleObj = game->CreateActor("Assets/SceneAssets/Title_Buttons");
		game->AddObject(titleObj);
	}*/
	auto stageSelectButtons = game->FindActor(mSelectNumberContainer[number]);
	if (stageSelectButtons != nullptr) {
		deleteButtonObj(mSelectNumberContainer[number]);
		auto titleObj = game->CreateActor(mSelectNumberContainer[number + 1]);
		game->AddObject(titleObj);
	}
	for (auto i = 0; i != count; i++)
	{
		auto selectNumber = i + number + 2;
		// ボタンの追加
		mCursorScript->AddButtonContainer(mSelectNumberContainer[selectNumber]);
		// 遷移先のシーンの追加
		mCursorScript->AddSceneContainer(mSelectNumberContainer[selectNumber + count]);
	}
	if(mTitleButtonCount != 0) mCursorScript->SetPosition();
	mTitleButtonCount += 1;
	//mTitleButtonCount = 0;
}

// ボタンオブジェクトの削除
void TitleMnager::deleteButtonObj(const char* name) {
	mCursorScript->ResetButtonContainer();
	auto buttonObj = game->FindActor(name);
	game->DestroyObject(buttonObj);
}