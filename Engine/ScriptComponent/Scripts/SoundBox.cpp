#include "SoundBox.h"
//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SoundBox::Initialize(){
	mIsSoundPlay = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SoundBox::Start(){

}

//���t���[���Ă΂�܂�
void SoundBox::Update(){
	auto sound = gameObject->GetComponent<SoundComponent>();
	if (!sound) return;
	// ���O���Ȃ���΂����폜����
	if (mName == "") game->DestroyObject(gameObject);
	// �Đ�
	if (!mIsSoundPlay) {
		sound->LoadFile(mName);
		if(!sound->IsPlay()) sound->Play();
		mIsSoundPlay = true;
	}
	//// �Đ����I���΍폜����(�ł��Ȃ�)
	//if(!sound->IsPlay()) game->DestroyObject(gameObject);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void SoundBox::Finish(){
	game->DestroyObject(gameObject);
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SoundBox::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SoundBox::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void SoundBox::OnCollideExit(Actor* target){
	(void)target;
}

// �T�E���h�̖��O��ݒ肵�܂�
void SoundBox::SetSoundName(const std::string name) {
	mName = "Assets/SceneAssets/SceneSound/" + name + ".wav";
	mIsSoundPlay = false;
}