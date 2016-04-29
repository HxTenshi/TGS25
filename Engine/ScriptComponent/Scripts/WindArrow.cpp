#include "WindArrow.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"
#include "Wind.h"
#include "SailBoard.h"
#include "Engine\DebugEngine.h"
#include<math.h>



//生成時に呼ばれます（エディター中も呼ばれます）
void WindArrow::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WindArrow::Start()
{
	mPlayer = game->FindActor("Board");
}

//毎フレーム呼ばれます
void WindArrow::Update(){

	if (mPlayer)
	{
		gameObject->mTransform->Position(mPlayer->mTransform->Position() + XMVectorSet(0.0f, 1.5f, 0.0f,1.0f));
	}
	auto wind = mPlayer->GetScript<SailBoard>();
	if (wind) {
		auto windvec = wind->GetWind();
		if (XMVector3Length(windvec).x > 0)
		{

			auto f = XMVectorSet(1, 0, 0, 1);
			f.y = 0;
			f = XMVector3Normalize(f);
			windvec = XMVector3Normalize(windvec);
			auto rpy = XMVector3Dot(f, windvec);
			auto r = acos(rpy.x);
			auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), r);
			gameObject->mTransform->Quaternion(rotatey);

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