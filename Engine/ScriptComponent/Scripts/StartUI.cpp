#include "StartUI.h"
#include"h_standard.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void StartUI::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void StartUI::Start(){

}

//���t���[���Ă΂�܂�
void StartUI::Update()
{
	if (mLimitTimer < 0)
	{
		game->DestroyObject(gameObject);
	}
	mLimitTimer -= game->DeltaTime()->GetDeltaTime();
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void StartUI::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void StartUI::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void StartUI::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void StartUI::OnCollideExit(Actor* target){
	(void)target;
}