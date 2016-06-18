#include "Fade.h"

//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"
#include "Engine\DebugEngine.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void Fade::Initialize(){
	mFadeInAlpha = 1.0f;
	mFadeOutAlpha = 0.0f;
	mIsFadeIn = false;
	mIsFadeOut = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Fade::Start(){
}

//毎フレーム呼ばれます
void Fade::Update(){
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Fade::Finish(){
}

//コライダーとのヒット時に呼ばれます
void Fade::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void Fade::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void Fade::OnCollideExit(Actor* target){
	(void)target;
}

void Fade::FadeIn(const float time) {
	// デルタタイムの取得
	auto deltaTime = game->DeltaTime()->GetDeltaTime();
	mFadeInAlpha -= 1.0f / time * deltaTime;
	if (mFadeInAlpha < 0.0f) mFadeInAlpha = 0.0f;
	// 描画
	auto mate = gameObject->GetComponent<MaterialComponent>();
	auto color = XMFLOAT4(0.0f, 0.0f, 0.0f, mFadeInAlpha);
	if (mate) mate->SetAlbedoColor(color);
	if (mFadeInAlpha == 0.0f) {
		mIsFadeIn = true;
		// game->DestroyObject(gameObject);
	}
}

// 
void Fade::FadeOut(const float time) {
	// デルタタイムの取得
	auto deltaTime = game->DeltaTime()->GetDeltaTime();
	if (mFadeOutAlpha > 1.0f) mFadeOutAlpha = 1.0f;
	else if(mFadeOutAlpha < 1.0f) mFadeOutAlpha += 1.0f / time * deltaTime;
	//game->Debug()->Log(std::to_string(mFadeOutAlpha));
	//mFadeOutAlpha = 0.0f;
	// 描画
	auto mate = gameObject->GetComponent<MaterialComponent>();
	//game->Debug()->Log(std::to_string(mate->mAlbedo.w));
	auto color = XMFLOAT4(0.0f, 0.0f, 0.0f, mFadeOutAlpha);
	mate->SetAlbedoColor(color);
	//if (mate) mate->SetAlbedoColor(color);
	if (mFadeOutAlpha == 1.0f) mIsFadeOut = true;
}

// フェードインをしたかを返します
bool Fade::IsFadeIn() {
	return mIsFadeIn;
}

// 
bool Fade::IsFadeOut() {
	return mIsFadeOut;
}