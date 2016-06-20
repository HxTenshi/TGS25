#include "StartUI.h"
#include"h_standard.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void StartUI::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void StartUI::Start(){

}

//毎フレーム呼ばれます
void StartUI::Update()
{
	if (mLimitTimer < 0)
	{
		game->DestroyObject(gameObject);
	}
	mLimitTimer -= game->DeltaTime()->GetDeltaTime();
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void StartUI::Finish(){

}

//コライダーとのヒット時に呼ばれます
void StartUI::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void StartUI::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void StartUI::OnCollideExit(Actor* target){
	(void)target;
}