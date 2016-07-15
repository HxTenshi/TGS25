#include "SoundBox.h"
//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void SoundBox::Initialize(){
	mIsSoundPlay = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void SoundBox::Start(){

}

//毎フレーム呼ばれます
void SoundBox::Update(){
	auto sound = gameObject->GetComponent<SoundComponent>();
	if (!sound) return;
	// 名前がなければすぐ削除する
	if (mName == "") game->DestroyObject(gameObject);
	// 再生
	if (!mIsSoundPlay) {
		sound->LoadFile(mName);
		if(!sound->IsPlay()) sound->Play();
		mIsSoundPlay = true;
	}
	//// 再生が終われば削除する(できない)
	//if(!sound->IsPlay()) game->DestroyObject(gameObject);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void SoundBox::Finish(){
	game->DestroyObject(gameObject);
}

//コライダーとのヒット時に呼ばれます
void SoundBox::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void SoundBox::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void SoundBox::OnCollideExit(Actor* target){
	(void)target;
}

// サウンドの名前を設定します
void SoundBox::SetSoundName(const std::string name) {
	mName = "Assets/SceneAssets/SceneSound/" + name + ".wav";
	mIsSoundPlay = false;
}