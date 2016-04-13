//#include "testdes.h"
#include "ScriptTemplate.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void testdes::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中は呼ばれません）
void testdes::Start(){

}

//毎フレーム呼ばれます
void testdes::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）
void testdes::Finish(){

}

//コライダーとのヒット時に呼ばれます
void testdes::OnCollide(Actor* target){
	(void)target;
}