#include "StageManager.h"
//#include "SailBoard.h"
//#include "Fade.h"
////�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"
#include "Input/Input.h"
#include "PlayerManager.h"
#include "SceneCursor.h"
#include "Fade.h"
#include "RetryScene.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void StageManager::Initialize(){
	mSetParentCount = 0;
	mButtonCreateCount = 0;
	mPauseCount = 0;
	mChangeTime = 2.0f * 60.0f;
	mSceneChangeTimer = 0.0f;
	mIsPause = false;
	mFadeOutObj = nullptr;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void StageManager::Start(){
	
}

//���t���[���Ă΂�܂�
void StageManager::Update(){
	if (mSetParentCount == 0) {
		// �v���C���[�}�l�[�W���Ɏ�������
		auto playerManagerObj = game->FindActor("PlayerManager");
		gameObject->mTransform->SetParent(playerManagerObj);
		mSetParentCount = 1;
	}
	// �v���C���[��T��
	auto playerManager = game->FindActor("PlayerManager");
	if (playerManager->Name() != "PlayerManager") return;
	auto playerManagerScript = playerManager->GetScript<PlayerManager>();
	// �t�F�[�h�I�u�W�F�N�g��T��
	auto fadeObj = game->FindActor("Fade");
	auto startObj = game->FindActor("Start");
	// �Q�[���J�n��ԁ@�������́@�t�F�[�h�I�u�W�F�N�g������ꍇ�Ȃ�΂����ɕԂ�
	if (!playerManagerScript->IsGameStart()) return;
	// �L�[����
	if (fadeObj == nullptr && startObj == nullptr) {
		if (Input::Trigger(PAD_DS4_KeyCoord::Button_OPTIONS) ||
			Input::Trigger(PAD_X_KeyCoord::Button_START) ||
			Input::Trigger(KeyCoord::Key_G)) {
			mPauseCount++;
			// �|�[�Y�̐����A�폜
			if (mPauseCount % 2 == 1) createPause();
			else deletePause();
		}
		else if (
			(Input::Trigger(PAD_DS4_KeyCoord::Button_CROSS) ||
				Input::Trigger(PAD_X_KeyCoord::Button_A) ||
				Input::Trigger(KeyCoord::Key_H)) &&
			mPauseCount % 2 == 1) {
			mPauseCount++;
			deletePause();
		}
		// �~�{�^����������Ă��|�[�Y��ʂ������悤�ɂ���
	}
	// �|�[�Y�łȂ���ΕԂ�
	if (!mIsPause) return;
	// �ĊJ�{�^���̏ꍇ�̓V�[���J�ڂ��Ȃ�
	if (mCursorScript->IsPushCursor() &&
		mCursorScript->GetButtonCount() == 0) {
		mPauseCount = 0;
		deletePause();
		return;
	}
	// �^�C�������ȏ�ɂȂ�����t�F�C�h�A�E�g����PV�V�[���Ɉڍs
	if (mChangeTime > mSceneChangeTimer) {
		if (mCursorScript->IsCursorMove()) mSceneChangeTimer = 0.0f;
		if (!mCursorScript->IsChangeScene())mSceneChangeTimer += 0.0167f;
	}
	// ����̃{�^���������ꂽ��V�[���J��
	if (mCursorScript->IsChangeScene() ||
		mChangeTime <= mSceneChangeTimer) {
		// �t�F�[�h�A�E�g���ăV�[���ړ�
		mCursorScript->SetIsCursorMove(true);
		// ��x��������
		if (mFadeOutObj == nullptr) {
			mFadeOutObj = game->CreateActor("Assets/Fade");
			game->AddObject(mFadeOutObj);
		}
		mFadeOutScript = mFadeOutObj->GetScript<Fade>();
		mFadeOutScript->FadeOut(mFadeOutSecond);
		// �t�F�[�h�A�E�g��V�[���ړ�
		if (mFadeOutScript->IsFadeOut()) {
			game->DeltaTime()->SetTimeScale(1.0f);
			// �V�[���J��ON
			if (mChangeTime <= mSceneChangeTimer) game->LoadScene("./Assets/Scenes/Title.scene");
			else {
				// ���X�^�[�g�̖��O����Ȃ�΁A�ړ��������^�C�g���ɂ���
				if (mRetryScene == "") mRetryScene = "Title";
				if (mCursorScript->GetButtonCount() == 2) mRetryScene = "Title";
				// ���X�^�[�g�I�u�W�F�̐���
				auto retryObj = game->CreateActor("Assets/RetrySceneObj");
				game->AddObject(retryObj);
				retryObj->mTransform->SetParent(gameObject);
				auto retryScript = retryObj->GetScript<RetryScene>();
				// ���X�^�[�g����V�[���̖��O������
				retryScript->SetRetrySceneName(mRetryScene);
				mCursorScript->OnChangeScene();
			}
		}
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void StageManager::Finish(){
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void StageManager::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void StageManager::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void StageManager::OnCollideExit(Actor* target){
	(void)target;
}

// �|�[�Y��ʂ̐���
void StageManager::createPause() {
	// �t�F�[�h�̍쐬
	auto fadeObj = game->CreateActor("Assets/Fade");
	game->AddObject(fadeObj);
	fadeObj->mTransform->SetParent(gameObject);
	fadeObj->Name("PauseFade");
	auto fadeScript = fadeObj->GetScript<Fade>();
	fadeScript->SetFadeAlpha(mFadeAlpha);
	// �{�^���{�b�N�X
	auto poseButtons = game->CreateActor("Assets/SceneAssets/Buttons");
	game->AddObject(poseButtons);
	poseButtons->mTransform->SetParent(gameObject);
	// �J�[�\��
	auto cursorObject = game->CreateActor("Assets/SceneAssets/SceneCursor");
	game->AddObject(cursorObject);
	cursorObject->mTransform->SetParent(gameObject);
	mCursorScript = cursorObject->GetScript<SceneCursor>();
	// �{�^���̒ǉ�
	mCursorScript->AddButtonContainer("Continue_Button");
	// ���g���C�i�������j
	mCursorScript->AddButtonContainer("Retry_Button");
	mCursorScript->AddButtonContainer("TitleBack_Button");
	// �J�ڐ�̃V�[���̒ǉ�
	mCursorScript->AddSceneContainer("NotMove");
	// �N���W�b�g(������)
	mCursorScript->AddSceneContainer("RetryScene");
	mCursorScript->AddSceneContainer("RetryScene");
	playPauseSE();
	game->DeltaTime()->SetTimeScale(0.0f);
	mSceneChangeTimer = 0.0f;
	mIsPause = true;
}

// �|�[�Y��ʂ̍폜
void StageManager::deletePause() {
	auto fadeObj = game->FindActor("PauseFade");
	if (fadeObj != nullptr) game->DestroyObject(fadeObj);
	game->DeltaTime()->SetTimeScale(1.0f);
	gameObject->mTransform->AllChildrenDestroy();
	playPauseSE();
	mIsPause = false;
}

// �|�[�Y��ʐ؂�ւ����̃T�E���h�Đ�
void StageManager::playPauseSE() {
	// �|�[�Y��ʂ̐؂�ւ�莞�ɖ炷
	auto sound = gameObject->GetComponent<SoundComponent>();
	if (!sound) return;
	sound->LoadFile("Assets/SceneAssets/SceneSound/Decision.wav");
	sound->Play();
}