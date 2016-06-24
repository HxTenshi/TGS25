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
		gameObject->mTransform->Position(mPlayer->mTransform->Position() + mPlayer->mTransform->Left() * 3 + XMVectorSet(0,2,0,0));
	}
	auto player = mPlayer->GetScript<SailBoard>();
	if (player) {
		auto windvec = player->GetWind();
		if (XMVector3Length(windvec).x > 0)
		{

			auto temp = XMVectorSet(1, 0, 0, 1);

			temp = XMVector3Normalize(temp);
			windvec = XMVector3Normalize(windvec);

			auto dot = XMVector3Dot(temp, windvec);
			auto radian = acos(dot.x);
			auto rotate = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), radian);
			gameObject->mTransform->Quaternion(rotate);

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