#include "HaneEffect.h"
#include"h_component.h"
#include"h_standard.h"
#include"PlayerManager.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void HaneEffect::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void HaneEffect::Start(){
	mScale = gameObject->mTransform->Scale();
	isEnd = false;
}

//毎フレーム呼ばれます
void HaneEffect::Update()
{
	if (isEnd)
	{
		mScale -= (XMVectorSet(1, 1, 0, 0) * 20);
		gameObject->mTransform->Scale(mScale);
		if (mScale.x < 100)
		{
			auto manager = game->FindActor("PlayerManager")->GetScript<PlayerManager>();
			manager->ItemGet();
			game->DestroyObject(gameObject);
		}
	}
	else
	{
		mPosition = XMVectorLerp(gameObject->mTransform->Position(), mSetPosition, mTimer);
		gameObject->mTransform->Position(mPosition);
		mTimer += game->DeltaTime()->GetNoScaleDeltaTime() * 0.5f;

		if (mTimer >= 1)
		{
			mScale += (XMVectorSet(1, 1, 0, 0) * 20);
			if (mScale.x > 200)
			{
				isEnd = true;
			}
			gameObject->mTransform->Scale(mScale);
		}
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void HaneEffect::Finish(){

}

//コライダーとのヒット時に呼ばれます
void HaneEffect::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void HaneEffect::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void HaneEffect::OnCollideExit(Actor* target){
	(void)target;
}

void HaneEffect::SetPosition(XMVECTOR pos)
{
	mSetPosition = pos;
}
