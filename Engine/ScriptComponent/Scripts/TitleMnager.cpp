#include "TitleMnager.h"
#include "Input/Input.h"
//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"PhysX\IPhysXEngine.h"

#include "SceneCursor.h"


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
	mCursorScript->AddButtonContainer("How_To_Button");
	mCursorScript->AddButtonContainer("GameEnd_Button");
	// シーンの追加
	mCursorScript->AddSceneContainer("Stage00");
	mCursorScript->AddSceneContainer("Stage00");
	mCursorScript->AddSceneContainer("Stage00");
	mCursorScript->AddSceneContainer("Stage00");
}

//毎フレーム呼ばれます
void TitleMnager::Update()
{
	auto camera = game->FindActor("MainCamera");
	auto cameraPosition = camera->mTransform->Position();
	auto cameraRotate = camera->mTransform->Rotate();
	// デルタタイムの取得
	auto deltaTime = game->DeltaTime()->GetDeltaTime();


	cameraRotate.y += cameraRotateSpeed * (3.14f / 180.0f) * (deltaTime * 60.0f);
	if (cameraRotate.y >= 3.14f * 2.0f) cameraRotate.y -= 3.14f * 2.0f;
	if (cameraRotate.y < 0.0f) cameraRotate.y += 3.14f * 2.0f;

	/*auto cameraPo = XMVectorSet(
		cameraPosition.x * sinf(cameraRotate.y), 0.0f,
		cameraPosition.z * cosf(cameraRotate.y), 0.0f);
	camera->mTransform->Position(cameraPo * 10.0f);*/
	camera->mTransform->Rotate(cameraRotate);

	//// カメラの回転
	//cameraRotate.y += cameraRotateSpeed * (3.14f / 180.0f) * (deltaTime * 60.0f);
	//if (cameraRotate.y >= 3.14f * 2.0f) cameraRotate.y -= 3.14f * 2.0f;
	//if (cameraRotate.y < 0.0f) cameraRotate.y += 3.14f * 2.0f;
	//camera->mTransform->Rotate(cameraRotate);
	// まだフェイドアウトなしです
	if(mCursorScript->IsChangeScene()) mCursorScript->OnButtonScene();

	/*if (Input::Trigger(PAD_DS4_KeyCoord::Button_CIRCLE) || Input::Trigger(KeyCoord::Key_J))
	{
		game->LoadScene("./Assets/Scenes/nishiWindSurfing.scene");
	}*/
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void TitleMnager::Finish(){

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