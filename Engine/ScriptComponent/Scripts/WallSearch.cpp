#include "WallSearch.h"
//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WallSearch::Initialize(){
	mIsWallHit = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WallSearch::Start(){

}

//���t���[���Ă΂�܂�
void WallSearch::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void WallSearch::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WallSearch::OnCollideBegin(Actor* target){
	if (IsNameHit(target)) mIsWallHit = true;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WallSearch::OnCollideEnter(Actor* target){
	if (IsNameHit(target)) mIsWallHit = true;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void WallSearch::OnCollideExit(Actor* target){
	if (IsNameHit(target)) mIsWallHit = false;
}

// �ǂɂԂ���������Ԃ��܂�
bool WallSearch::IsWallHit() {
	return mIsWallHit;
}

// �Ԃ������I�u�W�F�N�g���ړI�̃I�u�W�F�N�g�Ȃ�ture��Ԃ��܂�
bool WallSearch::IsNameHit(Actor* target) {
	if (target->Name() == "Tower" ||
		target->Name() == "bridge" ||
		target->Name() == "Tree") true;
	else false;
}
