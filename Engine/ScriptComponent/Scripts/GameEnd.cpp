#include "GameEnd.h"
#include "Game/Script/IGame.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GameEnd::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GameEnd::Start(){
}

//���t���[���Ă΂�܂�
void GameEnd::Update(){
	game->Shutdown();
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void GameEnd::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GameEnd::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GameEnd::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void GameEnd::OnCollideExit(Actor* target){
	(void)target;
}