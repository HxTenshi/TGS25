#include "DeltaTimeScale.h"

//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"

DeltaTimeScale::DeltaTimeScale(){
	mDeltaTimeScale = 1.0f;

}

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void DeltaTimeScale::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void DeltaTimeScale::Start(){

}

//���t���[���Ă΂�܂�
void DeltaTimeScale::Update(){
	game->DeltaTime()->SetTimeScale(mDeltaTimeScale);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void DeltaTimeScale::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void DeltaTimeScale::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void DeltaTimeScale::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void DeltaTimeScale::OnCollideExit(Actor* target){
	(void)target;
}