#include "RetryScene.h"
//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"

// �ÓI�����o�ϐ��̎���
std::string RetryScene::mRetrySceneName = "";

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void RetryScene::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void RetryScene::Start(){

}

//���t���[���Ă΂�܂�
void RetryScene::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void RetryScene::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void RetryScene::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void RetryScene::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void RetryScene::OnCollideExit(Actor* target){
	(void)target;
}

// ���X�^�[�g�V�[���̖��O�����܂�
void RetryScene::SetRetrySceneName(const std::string name) {
	mRetrySceneName = "./Assets/Scenes/" + name + ".scene";
}

// ���X�^�[�g�V�[���̖��O���擾���܂�
std::string RetryScene::GetRetrySceneName() {
	return mRetrySceneName;
}