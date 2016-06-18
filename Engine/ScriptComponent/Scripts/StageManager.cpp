#include "StageManager.h"
//#include "SailBoard.h"
#include "Fade.h"
////アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void StageManager::Initialize(){
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void StageManager::Start(){
}

//毎フレーム呼ばれます
void StageManager::Update(){
	auto player = game->FindActor("Board");
	//auto playerScript = player->GetScript<SailBoard>();
	auto deadline = game->FindActor("DeadLine");
	if (deadline == nullptr) return;
	if (player->mTransform->Position().y < deadline->mTransform->Position().y) {
		if (mFadeOutObj == nullptr) {
			mFadeOutObj = game->CreateActor("Assets/Fade");
			game->AddObject(mFadeOutObj);
		}
		auto mFadeOutScript = mFadeOutObj->GetScript<Fade>();
		mFadeOutScript->FadeOut(mFadeOutSecond);
		// フェードアウト後シーン移動
		if (mFadeOutScript->IsFadeOut()) game->LoadScene("Assets/Scenes/Title.scene");
	}
	//if(playerScript->)
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