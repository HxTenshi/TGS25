#include "Tornado.h"
#include"h_component.h"
#include"h_standard.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void Tornado::Initialize(){
	mTimer = 0.0f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Tornado::Start()
{
	mScale = gameObject->mTransform->Scale();
}

//毎フレーム呼ばれます
void Tornado::Update()
{
	auto mate = gameObject->GetComponent<MaterialComponent>();
	if (mate) {
		auto material = mate->GetMaterialPtr(0);
		material->mCBMaterial.mParam.MOffset.x += x;
		material->mCBMaterial.mParam.MOffset.y += y;
	}

	if (mTimer > LimitTime)
	{
		mScale -= XMVectorSet(0.01,0.01,0.01,0);
		gameObject->mTransform->Scale(mScale);
		if(mScale.x < 0) game->DestroyObject(gameObject);
	}

	mTimer += game->DeltaTime()->GetDeltaTime();

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