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
	mPastButtonCount = 0;
	mLerpTime = 0.0f;
	mIsMove = false;
	mIsCursorMove = false;
	mIsPushCursor = false;
	mIsChangeScene = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SceneCursor::Start(){
	auto buttonPosition = mButtonContainer[mButtonCount]->mTransform->Position();
	auto buttonScale = mButtonContainer[mButtonCount]->mTransform->Scale();
	auto scale = gameObject->mTransform->Scale();
	// �{�^���̔����̑傫�����擾����
	auto halfScale = XMVectorSet(
		buttonScale.x / 2.0f * (1.0f - buttonScale.x / 7000) + mAddCursorPositionX, 0.0f, 0.0f, 0.0f);
	// �ʒu�̕ύX
	gameObject->mTransform->Position(buttonPosition - halfScale);
	// �傫���̕ύX
	mButtonContainer[mButtonCount]->mTransform->Scale(buttonScale * 1.3f);
	if (mSetLerpTime <= 0) mSetLerpTime = 1;
}

//���t���[���Ă΂�܂�
void SceneCursor::Update(){
	// �ꐶ�����Ȃ�
	if (mIsMove) return;
	mIsPushCursor = false;
	mIsBackCursor = false;
	// �L�[����œ����ꍇ
	if (mIsCursorMove) {
		auto buttonPosition = mButtonContainer[mButtonCount]->mTransform->Position();
		auto buttonScale = mButtonContainer[mButtonCount]->mTransform->Scale();
		auto position = gameObject->mTransform->Position();
		auto scale = gameObject->mTransform->Scale();
		// �{�^���̔����̑傫�����擾����(�悭�킩��Ȃ��v�Z)
		auto halfScale = XMVectorSet(
			buttonScale.x / 2.0f * (1.0f - buttonScale.x / 7000) + mAddCursorPositionX, 0.0f, 0.0f, 0.0f);
		// ���`��Ԃ悤��
		if (mButtonCount != mPastButtonCount) {
			auto distance = buttonPosition - position;
			mLerp = (distance - halfScale) / mSetLerpTime;
			mPastButtonCount = mButtonCount;
		}
		// �ʒu�̕ύX
		if (mLerpTime != 1.0f) {
			auto deltaTime = game->DeltaTime()->GetDeltaTime();
			auto timeScale = game->DeltaTime()->GetTimeScale();
			auto moveVelocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			if (deltaTime == 0.0f) moveVelocity = mLerp * 0.0167f * mSpeed;
			else moveVelocity = mLerp * deltaTime * mSpeed;
			auto distance = buttonPosition - position;
			if (abs(moveVelocity.x) > abs(distance.x) ||
				abs(moveVelocity.y) > abs(distance.y))
				gameObject->mTransform->Position(buttonPosition - halfScale);
			else gameObject->mTransform->Position(position + moveVelocity);
			//mLerpTime += 1.0f / mSetLerpTime * deltaTime * mSpeed;
			auto lerpSpeed = 0.0f;
			if (deltaTime == 0.0f) lerpSpeed = 1.0f / mSetLerpTime * 0.0167f * mSpeed;
			else lerpSpeed = 1.0f / mSetLerpTime * deltaTime * mSpeed;
			mLerpTime += lerpSpeed;
			if (mLerpTime >= 1.0f) mLerpTime = 1.0f;
		}
		else{
			gameObject->mTransform->Position(buttonPosition - halfScale);
			mButtonContainer[mButtonCount]->mTransform->Scale(buttonScale * 1.3f);
			mLerpTime = 0.0f;
			mIsCursorMove = false;
		}
	}
	// �J�[�\���������I���܂œ��͏������s��Ȃ�
	if (mIsCursorMove) return;
	auto buttonScale = mButtonContainer[mButtonCount]->mTransform->Scale();
	// �V�[�����ς��Ȃ���͏������s��Ȃ�
	if (mIsChangeScene) return;
	// �L�[����
	if (Input::Trigger(PAD_DS4_KeyCoord::Button_UP) ||
		Input::Trigger(PAD_X_KeyCoord::Button_UP) ||
		Input::Trigger(KeyCoord::Key_A)) {
		if (mButtonCount == 0) return;
		mButtonContainer[mButtonCount]->mTransform->Scale(buttonScale / 1.3f);
		mButtonCount--;
		mIsCursorMove = true;
	}
	else if (Input::Trigger(PAD_DS4_KeyCoord::Button_DOWN) ||
		Input::Trigger(PAD_X_KeyCoord::Button_DOWN) ||
		Input::Trigger(KeyCoord::Key_Z)) {
		if (mButtonCount == mButtonContainer.size() - 1) return;
		mButtonContainer[mButtonCount]->mTransform->Scale(buttonScale / 1.3f);
		mButtonCount++;
		mIsCursorMove = true;
	}
	else if (Input::Trigger(PAD_DS4_KeyCoord::Button_CIRCLE) ||
		Input::Trigger(PAD_X_KeyCoord::Button_B) ||
		Input::Trigger(KeyCoord::Key_SPACE)) {
		mIsPushCursor = true;
		// �V�[���J�ڂ��Ȃ��ꍇ�͂����ɕԂ�
		if (mSceneContainer[mButtonCount] == "./Assets/Scenes/NotMove.scene") {
			return;
		}
		auto sound = gameObject->GetComponent<SoundComponent>();
		if (!sound) return;
		sound->LoadFile("Assets/SceneAssets/SceneSound/Decision.wav");
		sound->Play();
		// �V�[���̑J�� true
		mIsCursorMove = true;
		mIsChangeScene = true;
	}
	else if (Input::Trigger(PAD_DS4_KeyCoord::Button_CROSS) ||
		Input::Trigger(PAD_X_KeyCoord::Button_A) ||
		Input::Trigger(KeyCoord::Key_H))
		mIsBackCursor = true;
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

// �J�[�\���̈ړ������̐ݒ肵�܂�
void SceneCursor::SetIsCursorMove(bool isMove) {
	mIsMove = isMove;
}
// �J�[�\���̈ʒu��ύX���܂�
void SceneCursor::SetPosition() {
	auto buttonPosition = mButtonContainer[mButtonCount]->mTransform->Position();
	auto buttonScale = mButtonContainer[mButtonCount]->mTransform->Scale();
	auto halfScale = XMVectorSet(
		buttonScale.x / 2.0f * (1.0f - buttonScale.x / 7000) + mAddCursorPositionX, 0.0f, 0.0f, 0.0f);
	gameObject->mTransform->Position(buttonPosition - halfScale);
	mButtonContainer[mButtonCount]->mTransform->Scale(buttonScale * 1.3f);
}

// �{�^���R���e�i�����Z�b�g���܂�
void SceneCursor::ResetButtonContainer() {
	mButtonContainer.clear();
	mSceneContainer.clear();
	mButtonCount = 0;
}

// �{�^���̒l���擾���܂�
int SceneCursor::GetButtonCount() {
	return mButtonCount;
}

// �J�[�\���������Ă��邩��Ԃ��܂�
bool SceneCursor::IsCursorMove() {
	return mIsCursorMove;
}

// �J�[�\���������ꂽ����Ԃ��܂�
bool SceneCursor::IsPushCursor() {
	return mIsPushCursor;
}

// �J�[�\���Ŗ߂邩�ǂ�����Ԃ��܂�
bool SceneCursor::IsBackCursor() {
	return mIsBackCursor;
}

// �V�[�����ς��������Ԃ��܂�
bool SceneCursor::IsChangeScene() {
	return mIsChangeScene;
}