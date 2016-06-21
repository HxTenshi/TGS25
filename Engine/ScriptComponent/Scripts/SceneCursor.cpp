#include "SceneCursor.h"

//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"
#include "Engine\DebugEngine.h"

#include "Input/Input.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SceneCursor::Initialize(){
	mButtonCount = 0;
	mIsChangeScene = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SceneCursor::Start(){
	// �e�J�[�\���̑{��
	mCursorRightTop = game->FindActor("Cursor_Right_Top");
	mCursorRightBottom = game->FindActor("Cursor_Right_Bottom");
	mCursorLeftTop = game->FindActor("Cursor_Left_Top");
	mCursorLeftBottom = game->FindActor("Cursor_Left_Bottom");
	// �J�[�\���R���e�i�ɒǉ�
	mCursorContainer.push_back(mCursorRightTop);
	mCursorContainer.push_back(mCursorRightBottom);
	mCursorContainer.push_back(mCursorLeftTop);
	mCursorContainer.push_back(mCursorLeftBottom);
	// �J�[�\�����W�R���e�i�ɒǉ�
	mCursorVelocityContainer.push_back(XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f));
	mCursorVelocityContainer.push_back(XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f));
	mCursorVelocityContainer.push_back(XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f));
	mCursorVelocityContainer.push_back(XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f));
}

//���t���[���Ă΂�܂�
void SceneCursor::Update(){
	// �V�[�����ς��Ȃ���͏������s��Ȃ�
	if (mIsChangeScene) return;
	// �J�[�\���̈ʒu�Ƒ傫���̕ύX
	for (auto i = 0; i != 4; i++) {
		auto cursor = mCursorContainer[i];
		// �ʒu���{�^���ɍ��킹��
		auto buttonPosition = mButtonContainer[mButtonCount]->mTransform->Position();
		auto buttonScale = mButtonContainer[mButtonCount]->mTransform->Scale();
		// �{�^���̔����̑傫�����擾����
		auto halfScale = XMVectorSet(
			buttonScale.x / 2.0f + mAddCursorPositionX,
			buttonScale.y / 2.0f + mAddCursorPositionY, 0.0f, 0.0f);
		// �ʒu�̕ύX
		cursor->mTransform->Position(
			buttonPosition + (halfScale * mCursorVelocityContainer[i]));
		// �傫���̕ύX
		auto cursorScale = XMVectorSet(mCursorScaleX, mCursorScaleY, 0.0f, 0.0f);
		cursor->mTransform->Scale(cursorScale);
	}
	// �L�[����
	if (Input::Trigger(PAD_DS4_KeyCoord::Button_UP) || Input::Trigger(KeyCoord::Key_A)) {
		if (mButtonCount > 0) {
			mButtonCount--;
		}
	}
	else if (Input::Trigger(PAD_DS4_KeyCoord::Button_DOWN) || Input::Trigger(KeyCoord::Key_Z)) {
		if (mButtonCount < mButtonContainer.size() - 1) {
			mButtonCount++;
		}
	}
	else if (Input::Trigger(PAD_DS4_KeyCoord::Button_CIRCLE) || Input::Trigger(KeyCoord::Key_SPACE)) {
		mIsChangeScene = true;
		auto sound = gameObject->GetComponent<SoundComponent>();
		if (!sound) return;
		sound->LoadFile("Assets/SceneTexture/Decision.wav");
		sound->Play();
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void SceneCursor::Finish(){
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SceneCursor::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SceneCursor::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void SceneCursor::OnCollideExit(Actor* target){
	(void)target;
}

// �{�^���̔z��ɒǉ����܂�
void SceneCursor::AddButtonContainer(const char* buttonName) {
	mButtonContainer.push_back(game->FindActor(buttonName));
}

// �V�[���̔z��ɒǉ����܂�
void SceneCursor::AddSceneContainer(const std::string sceneName) {
	std::string addSceneName = "./Assets/Scenes/" + sceneName + ".scene";
	mSceneContainer.push_back(addSceneName);
}

// �����ꂽ�{�^���ɂ���ăV�[�����ړ����܂�
void SceneCursor::OnChangeScene() {
	game->LoadScene(mSceneContainer[mButtonCount]);
}

// �{�^���̒l���擾���܂�
int SceneCursor::GetButtonCount() {
	return mButtonCount;
}

// �V�[�����ς��������Ԃ��܂�
bool SceneCursor::IsChangeScene() {
	return mIsChangeScene;
}