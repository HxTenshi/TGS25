#include "GameQuit.h"
#include"h_standard.h"
#include"h_component.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GameQuit::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GameQuit::Start(){

}

//���t���[���Ă΂�܂�
void GameQuit::Update()
{
	if (Input::Down(PAD_X_KeyCoord::Button_BACK) && Input::Down(PAD_X_KeyCoord::Button_START))
	{
		game->Shutdown();
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void GameQuit::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GameQuit::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GameQuit::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void GameQuit::OnCollideExit(Actor* target){
	(void)target;
}