#include "WindArrow.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"
#include "Wind.h"



//生成時に呼ばれます（エディター中も呼ばれます）
void WindArrow::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WindArrow::Start(){

}

//毎フレーム呼ばれます
void WindArrow::Update(){

	auto wind = game->FindActor("Wind");
	if (wind){
		auto windscr = wind->GetScript<Wind>();
		if (windscr){
			auto v = windscr->GetWindVelocity();
			if (XMVector3Length(v).x > 0){

				auto f = gameObject->mTransform->GetParent()->mTransform->Forward();
				f.y = 0;
				f = XMVector3Normalize(f);
				v = XMVector3Normalize(v);
				auto rpy = XMVector3AngleBetweenNormals(f, v);
				gameObject->mTransform->Rotate(XMVectorSet(0, rpy.x, 0, 1));

			}
		}
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void WindArrow::Finish(){

}

//コライダーとのヒット時に呼ばれます
void WindArrow::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void WindArrow::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void WindArrow::OnCollideExit(Actor* target){
	(void)target;
}