#include "MoveSmoke.h"
#include "../h_standard.h"
#include "../h_component.h"
#include <cmath>
#include <algorithm>
#include <math.h>

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void MoveSmoke::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void MoveSmoke::Start(){

}

//���t���[���Ă΂�܂�
void MoveSmoke::Update(){

	auto com = gameObject->GetComponent<ParticleComponent>();
	if (!com)return;
	if (mMaxSpeed == 0.0)return;
	//auto mStartParam = com->mCParticleBuffer.mParam;
	com->mParticleParam .Vector.y = 0.2f * (mSpeed / mMaxSpeed);
	com->mParticleParam.Param.x = 128 * (mSpeed / mMaxSpeed);
	com->mParticleParam.MinMaxScale.x = 0.5 *(mSpeed / mMaxSpeed) + 0.5;
	com->mParticleParam.MinMaxScale.y = 1.5 *(mSpeed / mMaxSpeed) + 0.5;

	
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void MoveSmoke::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void MoveSmoke::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void MoveSmoke::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void MoveSmoke::OnCollideExit(Actor* target){
	(void)target;
}