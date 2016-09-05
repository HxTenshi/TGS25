#include "GameQuit.h"
#include"h_standard.h"
#include"h_component.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void GameQuit::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void GameQuit::Start(){

}

//毎フレーム呼ばれます
void GameQuit::Update()
{
	if (Input::Down(PAD_X_KeyCoord::Button_BACK) && Input::Down(PAD_X_KeyCoord::Button_START))
	{
		game->Shutdown();
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void GameQuit::Finish(){

}

//コライダーとのヒット時に呼ばれます
void GameQuit::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void GameQuit::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void GameQuit::OnCollideExit(Actor* target){
	(void)target;
}