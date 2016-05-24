#include "JumpDai.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Engine\DebugEngine.h"
#include "Game/Component/PhysXComponent.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void JumpDai::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void JumpDai::Start(){

}

//毎フレーム呼ばれます
void JumpDai::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void JumpDai::Finish(){

}

//コライダーとのヒット時に呼ばれます
void JumpDai::OnCollideBegin(Actor* target){
	if (target->Name() != "Board")return;
	auto physx = target->GetComponent<PhysXComponent>();
	if (physx){
		auto v = physx->GetForceVelocity();
		
		target->mTransform->AddForce(XMVector3Normalize(v) *mJumpPower + XMVectorSet(0, mJumpUPPower, 0, 0), ForceMode::eIMPULSE);
		game->Debug()->Log("busst");
	}
}

//コライダーとのヒット中に呼ばれます
void JumpDai::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void JumpDai::OnCollideExit(Actor* target){
	(void)target;
}