#include "RainbowShiningWing.h"
//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void RainbowShiningWing::Initialize(){
	mColorNumber = 0;
	mRedColor = 1.0f;
	mGreenColor = 0.0f;
	mBlueColor = 0.0f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void RainbowShiningWing::Start(){
	// カラーコンテナに追加
	mColorContainer.push_back(mGreenColor);
	mColorContainer.push_back(mRedColor);
	mColorContainer.push_back(mBlueColor);
}

//毎フレーム呼ばれます
void RainbowShiningWing::Update(){
	//auto particle = gameObject->GetComponent<ParticleComponent>();
	auto deltaTime = game->DeltaTime()->GetDeltaTime();
	// 0の場合は加算、1の場合は減算
	if (mColorNumber % 2 == 0) {
		mColorContainer[mColorNumber % 3] += 1.0f / 256 * deltaTime * (10.0f * mColorChangeSpeed);
		if (mColorContainer[mColorNumber % 3] > 1.0f) mColorContainer[mColorNumber % 3] = 1.0f;
		if (mColorContainer[mColorNumber % 3] == 1.0f) mColorNumber++;
	}
	else {
		mColorContainer[mColorNumber % 3] -= 1.0f / 256 * deltaTime * (10.0f * mColorChangeSpeed);
		if (mColorContainer[mColorNumber % 3] < 0.0f) mColorContainer[mColorNumber % 3] = 0.0f;
		if (mColorContainer[mColorNumber % 3] == 0.0f) mColorNumber++;
	}
	// 色の変更
	auto material = gameObject->GetComponent<MaterialComponent>();
	auto color = XMFLOAT4(mColorContainer[1], mColorContainer[0], mColorContainer[2], mColorAlpha);
	if (material) material->SetAlbedoColor(color);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void RainbowShiningWing::Finish(){
	game->DestroyObject(gameObject);
}

//コライダーとのヒット時に呼ばれます
void RainbowShiningWing::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void RainbowShiningWing::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void RainbowShiningWing::OnCollideExit(Actor* target){
	(void)target;
}