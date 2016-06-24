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
	mIsChangeScene = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SceneCursor::Start(){
	auto buttonPosition = mButtonContainer[mButtonCount]->mTransform->Position();
	auto buttonScale = mButtonContainer[mButtonCount]->mTransform->Scale();
	// �{�^���̔����̑傫�����擾����
	auto halfScale = XMVectorSet(
		buttonScale.x / 2.0f + mAddCursorPositionX, 0.0f, 0.0f, 0.0f);
	// �ʒu�̕ύX
	gameObject->mTransform->Position(buttonPosition - halfScale);
	if (mSetLerpTime <= 0) mSetLerpTime = 1;
}

//���t���[���Ă΂�܂�
void SceneCursor::Update(){
	if (mIsCursorMove) {
		auto buttonPosition = mButtonContainer[mButtonCount]->mTransform->Position();
		auto buttonScale = mButtonContainer[mButtonCount]->mTransform->Scale();
		auto position = gameObject->mTransform->Position();
		// �{�^���̔����̑傫�����擾����
		auto halfScale = XMVectorSet(
			buttonScale.x / 2.0f + mAddCursorPositionX, 0.0f, 0.0f, 0.0f);
		// ���`��Ԃ悤��
		if (mButtonCount != mPastButtonCount) {
			auto distance = buttonPosition - position;
			/*auto absDistance = 
			auto nomeVector = XMVector3Normalize(position - buttonPosition);*/
			mLerp = (distance - halfScale) / mSetLerpTime;
			mPastButtonCount = mButtonCount;
		}
		//game->Debug()->Log(std::to_string(mLerpTime));
		// �ʒu�̕ύX
		if (mLerpTime != 1.0f) {
			auto deltaTime = game->DeltaTime()->GetDeltaTime();
			gameObject->mTransform->Position(position + mLerp * deltaTime);
			mLerpTime += 1.0f / mSetLerpTime * deltaTime;
			if (mLerpTime >= 1.0f) mLerpTime = 1.0f;
		}
		else{
			gameObject->mTransform->Position(buttonPosition - halfScale);
			mLerpTime = 0.0f;
			mIsCursorMove = false;
		}
	}
	// �J�[�\���������I���܂œ��͏������s��Ȃ�
	if (mIsCursorMove) return;
	// �V�[�����ς��Ȃ���͏������s��Ȃ�
	if (mIsChangeScene) return;
	
	// �L�[����
	if (Input::Trigger(PAD_DS4_KeyCoord::Button_UP) ||
		Input::Trigger(KeyCoord::Key_A)) {
		//if (mButtonCount > 0) mButtonCount--;
		if (mButtonCount == 0) return;
		mButtonCount--;
		mIsCursorMove = true;
	}
	else if (Input::Trigger(PAD_DS4_KeyCoord::Button_DOWN) ||
		Input::Trigger(KeyCoord::Key_Z)) {
		//if (mButtonCount < mButtonContainer.size() - 1) mButtonCount++;
		if (mButtonCount == mButtonContainer.size() - 1) return;
		mButtonCount++;
		mIsCursorMove = true;
	}
	else if (Input::Trigger(PAD_DS4_KeyCoord::Button_CIRCLE) ||
		Input::Trigger(KeyCoord::Key_SPACE)) {
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