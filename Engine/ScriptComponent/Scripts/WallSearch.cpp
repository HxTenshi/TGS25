#include "WallSearch.h"
//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void WallSearch::Initialize(){
	mIsWallHit = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WallSearch::Start(){

}

//毎フレーム呼ばれます
void WallSearch::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void WallSearch::Finish(){

}

//コライダーとのヒット時に呼ばれます
void WallSearch::OnCollideBegin(Actor* target){
	if (IsNameHit(target)) mIsWallHit = true;
}

//コライダーとのヒット中に呼ばれます
void WallSearch::OnCollideEnter(Actor* target){
	if (IsNameHit(target)) mIsWallHit = true;
}

//コライダーとのロスト時に呼ばれます
void WallSearch::OnCollideExit(Actor* target){
	if (IsNameHit(target)) mIsWallHit = false;
}

// 壁にぶつかったかを返します
bool WallSearch::IsWallHit() {
	return mIsWallHit;
}

// ぶつかったオブジェクトが目的のオブジェクトならtureを返します
bool WallSearch::IsNameHit(Actor* target) {
	if (target->Name() == "Tower" ||
		target->Name() == "bridge" ||
		target->Name() == "Tree") true;
	else false;
}
