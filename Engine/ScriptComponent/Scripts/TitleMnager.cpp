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


//生成時に呼ばれます（エディター中も呼ばれます）
void TitleMnager::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void TitleMnager::Start(){
	// カーソルの取得
	mCursorObj = game->FindActor("SceneCursor");
	mCursorScript = mCursorObj->GetScript<SceneCursor>();
	// ボタンの追加
	mCursorScript->AddButtonContainer("GameStart_Button");
	mCursorScript->AddButtonContainer("Credit_Button");
	mCursorScript->AddButtonContainer("How_To_Play_Button");
	mCursorScript->AddButtonContainer("GameEnd_Button");
	// 遷移先のシーンの追加
	mCursorScript->AddSceneContainer("Stage00");
	mCursorScript->AddSceneContainer("Stage00");
	mCursorScript->AddSceneContainer("Stage00");
	mCursorScript->AddSceneContainer("Stage00");
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

	// フェードアウトしてシーン移動
	if (mCursorScript->IsChangeScene()) {
		// 一度だけ生成
		if (mFadeOutObj == nullptr) {
			mFadeOutObj = game->CreateActor("Assets/Fade");
			game->AddObject(mFadeOutObj);
		}
		mFadeOutScript = mFadeOutObj->GetScript<Fade>();
		mFadeOutScript->FadeOut(mFadeOutSecond);
		// フェードアウト後シーン移動
		if (mFadeOutScript->IsFadeOut()) mCursorScript->OnChangeScene();
	}

	/*if (Input::Trigger(PAD_DS4_KeyCoord::Button_CIRCLE) || Input::Trigger(KeyCoord::Key_J))
	{
		game->LoadScene("./Assets/Scenes/nishiWindSurfing.scene");
	}*/

	/*auto centerPosition = center->mTransform->Position();
	auto v = centerPosition - cameraPosition;
	auto angle = atan2(v.x, v.z);
	auto escapeAngle = angle;*/
	//auto quaternion = XMQuaternionRotationAxis(camera->mTransform->Up(), angle);
	//// angleに90°加算する
	//auto escapeAngle = angle + 3.141593f / 2.0f;
	/*auto escapeAngle = angle + 3.141593f;
	if (std::to_string(escapeAngle) == "0.000000") escapeAngle = 0.0f;
	if (escapeAngle < 0) escapeAngle += 3.141593f * 2.0f;*/
	// カメラの親の移動
	camera->mTransform->Position(
		cameraPosition +
		((camera->mTransform->Left() * 0.01f) * 
			mCameraRotateSpeed * (deltaTime * 60.0f)));
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