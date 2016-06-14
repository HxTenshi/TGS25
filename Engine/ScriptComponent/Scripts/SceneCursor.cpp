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

}

//���t���[���Ă΂�܂�
void SceneCursor::Update(){
	// �����̐F��ǉ�����
	if (!mIsSetColor) {
		for (auto i = 0; i != mButtonContainer.size(); ++i) {
			auto mate = mButtonContainer[i]->GetComponent<MaterialComponent>();
			mInitColorContainer.push_back(mate->mAlbedo);
		}
		mIsSetColor = true;
	}
	// �ʒu�A�傫�����{�^���ɍ��킹��
	auto buttonPosition = mButtonContainer[mButtonCount]->mTransform->Position();
	auto buttonScale = mButtonContainer[mButtonCount]->mTransform->Scale();
	gameObject->mTransform->Position(buttonPosition);
	gameObject->mTransform->Scale(buttonScale);
	// ���ݐG��Ă���{�^�����Â�����
	auto mate = mButtonContainer[mButtonCount]->GetComponent<MaterialComponent>();
	auto color = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	if (mate) mate->SetAlbedoColor(color);
	// �V�[�����ς��Ȃ���͂��Ȃ�
	if (mIsChangeScene) return;
	//auto begin = mButtonContainer.begin();
	if (Input::Trigger(KeyCoord::Key_A)) {
		if (mButtonCount > 0) {
			if(mate) mate->SetAlbedoColor(mInitColorContainer[mButtonCount]);
			mButtonCount--;
		}
	}
	else if (Input::Trigger(KeyCoord::Key_Z)) {
		if (mButtonCount < mButtonContainer.size() - 1) {
			if (mate) mate->SetAlbedoColor(mInitColorContainer[mButtonCount]);
			mButtonCount++;
		}
	}
	if (Input::Trigger(KeyCoord::Key_SPACE)) {
		mIsChangeScene = true;
		game->Debug()->Log("���̃V�[���ړ��͂܂����܂���");
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
	/*auto mate = buttonObj->GetComponent<MaterialComponent>();
	mInitColorContainer.push_back(mate->mAlbedo);*/
}

// �V�[���̔z��ɒǉ����܂�
void SceneCursor::AddSceneContainer(const std::string sceneName) {
	std::string addSceneName = "./Assets/Scenes/" + sceneName + ".scene";
	mSceneContainer.push_back(addSceneName);
}

// �����ꂽ�{�^���ɂ���ăV�[�����ړ����܂�
void SceneCursor::OnButtonScene() {
	game->LoadScene(mSceneContainer[mButtonCount]);
}

// 
int SceneCursor::GetButtonCount() {
	return mButtonCount;
}

// 
bool SceneCursor::IsChangeScene() {
	return mIsChangeScene;
}