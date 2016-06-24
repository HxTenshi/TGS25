#include "HowToManager.h"
//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"
#include "Input/Input.h"
#include "Game\Component\MovieComponent.h"
#include "Fade.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void HowToManager::Initialize(){
	mMovieTimer = 0.0f;
	mIsChangeScene = false;
	mIsFadeIn = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void HowToManager::Start(){
}

//���t���[���Ă΂�܂�
void HowToManager::Update(){
	if (!mIsFadeIn) {
		mFadeOutObj = game->FindActor("Fade");
		if (mFadeOutObj == nullptr) mIsFadeIn = true;
		auto fadeInScript = mFadeOutObj->GetScript<Fade>();
		fadeInScript->FadeIn(mFadeOutSecond);
		if (fadeInScript->IsFadeIn()) {
			game->DestroyObject(mFadeOutObj);
			mFadeOutObj = nullptr;
			mIsFadeIn = true;
		}
	}
	else {
		// ���[�r�[�Đ�
		auto movie = gameObject->GetComponent<MovieComponent>();
		if (mMovieTimer >= 47.5f) {
			movie->Stop();
			mIsChangeScene = true;
		}
		else {
			movie->Play();
			movie->SetLoop(false);
			mMovieTimer += game->DeltaTime()->GetDeltaTime();
		}
		//if (!movie->IsPlay()) mIsChangeScene = true;
		// �Đ�������������^�C�g���V�[���ɑJ��
		// ��x��������
		if (mIsChangeScene) {
			if (mFadeOutObj == nullptr) {
				mFadeOutObj = game->CreateActor("Assets/Fade");
				game->AddObject(mFadeOutObj);
			}
			auto mFadeOutScript = mFadeOutObj->GetScript<Fade>();
			mFadeOutScript->FadeOut(mFadeOutSecond);
			// �t�F�[�h�A�E�g��V�[���ړ�
			if (mFadeOutScript->IsFadeOut()) {
				movie->Stop();
				game->LoadScene("Assets/Scenes/Title.scene");
			}
		}
		// �{�^���������ꂽ���x�Ɖ����Ȃ�
		if (mIsChangeScene) return;
		// �{�^���������ꂽ�狭���I���H
		if (Input::Trigger(PAD_DS4_KeyCoord::Button_CIRCLE) || Input::Trigger(KeyCoord::Key_SPACE)) {
			mIsChangeScene = true;
			auto sound = gameObject->GetComponent<SoundComponent>();
			if (!sound) return;
			sound->LoadFile("Assets/SceneTexture/Decision.wav");
			sound->Play();
		}
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void HowToManager::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void HowToManager::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void HowToManager::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void HowToManager::OnCollideExit(Actor* target){
	(void)target;
}