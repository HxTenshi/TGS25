#include "CameraController.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"
#include "Engine\DebugEngine.h"
#include<math.h>


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
	Look();

	mPosition = mTarget->mTransform->Position() + (mTarget->mTransform->Forward() * -3) + XMVectorSet(0,1,0,1);

	//mPosition = mTarget->mTransform->Position() + XMVectorSet(0, 1, -5, 0);
	gameObject->mTransform->Position(mPosition);
	//gameObject->mTransform->Rotate(XMQuaternionRotationMatrix(mat));
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

void CameraController::Look()
{
	auto SubVector = gameObject->mTransform->Position() - mTarget->mTransform->Position();

	auto Angel = atan2(-SubVector.x, -SubVector.z);
	if (Input::Down(KeyCoord::Key_F))
	game->Debug()->Log(std::to_string(Angel));

	auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), Angel);
	auto vec = XMVectorSet(0, Angel, 0, 1);
	gameObject->mTransform->Rotate(vec);

}
