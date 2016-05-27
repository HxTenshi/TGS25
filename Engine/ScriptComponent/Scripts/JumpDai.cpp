#include "JumpDai.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Engine\DebugEngine.h"
#include "Game/Component/PhysXComponent.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void JumpDai::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void JumpDai::Start(){

}

//���t���[���Ă΂�܂�
void JumpDai::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void JumpDai::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void JumpDai::OnCollideBegin(Actor* target){
	if (target->Name() != "Board")return;
	auto physx = target->GetComponent<PhysXComponent>();
	if (physx){
		auto v = physx->GetForceVelocity();
		
		target->mTransform->AddForce(XMVector3Normalize(v) *mJumpPower + XMVectorSet(0, mJumpUPPower, 0, 0), ForceMode::eIMPULSE);
		game->Debug()->Log("busst");
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void JumpDai::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void JumpDai::OnCollideExit(Actor* target){
	(void)target;
}