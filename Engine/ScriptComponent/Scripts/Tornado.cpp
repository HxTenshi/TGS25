#include "Tornado.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void Tornado::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Tornado::Start(){

}

//毎フレーム呼ばれます
void Tornado::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Tornado::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Tornado::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void Tornado::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void Tornado::OnCollideExit(Actor* target){
	(void)target;
}