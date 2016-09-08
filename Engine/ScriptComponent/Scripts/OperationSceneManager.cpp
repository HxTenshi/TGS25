#include "OperationSceneManager.h"
#include "Fade.h"
//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void OperationSceneManager::Initialize(){
	mIsChangeScene = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void OperationSceneManager::Start(){
	mFadeObj = game->CreateActor("Assets/Fade");
	game->AddObject(mFadeObj);
}

//���t���[���Ă΂�܂�
void OperationSceneManager::Update(){
	auto fadeInScript = mFadeObj->GetScript<Fade>();
	fadeInScript->FadeIn(mFadeSecond);
	if (!fadeInScript->IsFadeIn()) return;

	if (Input::Trigger(PAD_DS4_KeyCoord::Button_CROSS) ||
		Input::Trigger(PAD_X_KeyCoord::Button_A) ||
		Input::Trigger(KeyCoord::Key_H)) {
		mIsChangeScene = true;
		auto sound = gameObject->GetComponent<SoundComponent>();
		if (!sound) return;
		sound->LoadFile("Assets/SceneAssets/SceneSound/Decision.wav");
		sound->Play();
	}
	// �{�^����������Ȃ���Ζ߂�Ȃ�
	if (!mIsChangeScene) return;
	// ��x��������
	if (mFadeObj == nullptr) {
		mFadeObj = game->CreateActor("Assets/Fade");
		game->AddObject(mFadeObj);
	}
	auto fadeOutScript = mFadeObj->GetScript<Fade>();
	fadeOutScript->FadeOut(mFadeSecond);
	// �t�F�[�h�A�E�g��V�[���ړ�
	if (fadeOutScript->IsFadeOut())
		game->LoadScene("./Assets/Scenes/Title.scene");
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void OperationSceneManager::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void OperationSceneManager::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void OperationSceneManager::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void OperationSceneManager::OnCollideExit(Actor* target){
	(void)target;
}