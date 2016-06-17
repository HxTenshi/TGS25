#include "GameEnd.h"
#include "Game/Script/IGame.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void GameEnd::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void GameEnd::Start(){
}

//毎フレーム呼ばれます
void GameEnd::Update(){
	game->Shutdown();
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void GameEnd::Finish(){

}

//コライダーとのヒット時に呼ばれます
void GameEnd::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void GameEnd::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void GameEnd::OnCollideExit(Actor* target){
	(void)target;
}