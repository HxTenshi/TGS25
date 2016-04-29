#include "CameraController.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"SailBoard.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void CameraController::Initialize(){
	

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void CameraController::Start(){
	mTarget = game->FindActor("Board");
	gameObject->mTransform->Position(XMVectorSet(0, 0, 0, 1));
}

//毎フレーム呼ばれます
void CameraController::Update()
{
	if (mTarget)
	{
		Look();
		if (!mTarget->GetScript<SailBoard>()->GetIsJump())
		{
			mPosition = mTarget->mTransform->Position() + (mTarget->mTransform->Forward() * -3) + XMVectorSet(0, 1, 0, 1);
		}
		else
		{
			mPosition = mTarget->mTransform->Position() + XMVectorSet(0, 1, -5, 1);
		}
		//mPosition = mTarget->mTransform->Position() + XMVectorSet(0, 1, -5, 0);
		gameObject->mTransform->Position(mPosition);
		//gameObject->mTransform->Rotate(XMQuaternionRotationMatrix(mat));
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void CameraController::Finish(){

}

//コライダーとのヒット時に呼ばれます
void CameraController::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void CameraController::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void CameraController::OnCollideExit(Actor* target){
	(void)target;
}

//プレイヤーを見る
void CameraController::Look()
{
	auto SubVector = gameObject->mTransform->Position() - mTarget->mTransform->Position();

	auto Angel = atan2(-SubVector.x, -SubVector.z);

	auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), Angel);
	auto vec = XMVectorSet(0, Angel, 0, 1);
	gameObject->mTransform->Rotate(vec);

}
