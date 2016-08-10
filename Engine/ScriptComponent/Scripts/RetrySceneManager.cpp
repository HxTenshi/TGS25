#include "RetrySceneManager.h"
//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"

#include "RetryScene.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void RetrySceneManager::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void RetrySceneManager::Start(){
	// ���X�^�[�g�I�u�W�F�̐���
	auto retryObj = game->CreateActor("Assets/RetrySceneObj");
	game->AddObject(retryObj);
	auto retryScript = retryObj->GetScript<RetryScene>();
	mSceneName = retryScript->GetRetrySceneName();
}

//���t���[���Ă΂�܂�
void RetrySceneManager::Update(){
	// �V�[���ړ�
	if (mSceneName == "") 
		mSceneName = "./Assets/Scenes/Title.scene";
	game->LoadScene(mSceneName);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void RetrySceneManager::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void RetrySceneManager::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void RetrySceneManager::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void RetrySceneManager::OnCollideExit(Actor* target){
	(void)target;
}