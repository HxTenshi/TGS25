#include "TitleMnager.h"
#include "Input/Input.h"
//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"PhysX\IPhysXEngine.h"
// �J�[�\���ƃt�F�[�h�ǉ�
#include "SceneCursor.h"
#include "Fade.h"
#include "HowToManager.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TitleMnager::Initialize(){
	mTitleButtonCount = 0;
	mSceneChangeTimer = 0.0f;
	// �^�C�g���Z���N�g
	mSelectNumberContainer.push_back("StageSelectButtons");
	mSelectNumberContainer.push_back("Assets/SceneAssets/Title_Buttons");
	mSelectNumberContainer.push_back("GameStart_Button");
	mSelectNumberContainer.push_back("How_To_Button");
	mSelectNumberContainer.push_back("GameEnd_Button");
	mSelectNumberContainer.push_back("NotMove");
	mSelectNumberContainer.push_back("How_To");
	mSelectNumberContainer.push_back("GameEnd");
	// �X�e�[�W�Z���N�g
	mSelectNumberContainer.push_back("Title_Buttons");
	mSelectNumberContainer.push_back("Assets/SceneAssets/StageSelectButtons");
	mSelectNumberContainer.push_back("Stage01_Button");
	mSelectNumberContainer.push_back("Stage02_Button");
	mSelectNumberContainer.push_back("Stage03_Button");
	mSelectNumberContainer.push_back("FastStage");
	mSelectNumberContainer.push_back("Stage01");
	mSelectNumberContainer.push_back("Stage02");
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TitleMnager::Start(){
	// �J�[�\���̎擾
	mCursorObj = game->FindActor("SceneCursor");
	mCursorScript = mCursorObj->GetScript<SceneCursor>();
	// �{�^���̒ǉ�
	createButton(TITLESELECT_NUMBER, TITLESELECT_COUNT);
}

//���t���[���Ă΂�܂�
void TitleMnager::Update()
{
	auto center = game->FindActor("CenterPosition");
	auto camera = game->FindActor("MainCameraParent");
	auto cameraPosition = camera->mTransform->Position();
	auto cameraRotate = camera->mTransform->Rotate();
	// �f���^�^�C���̎擾
	auto deltaTime = game->DeltaTime()->GetDeltaTime();

	if (mTitleButtonCount % 2 == 1) {
		if (mCursorScript->IsPushCursor() &&
			mCursorScript->GetButtonCount() == 0)
			createButton(STAGESELECT_NUMBER, STAGESELECT_COUNT);
	}
	else {
		if (mCursorScript->IsBackCursor())
			createButton(TITLESELECT_NUMBER, TITLESELECT_COUNT);
	}
	// createButton()

	// �������͂��Ȃ���΃^�C�����Z
	// �^�C�������ȏ�ɂȂ�����t�F�C�h�A�E�g����PV�V�[���Ɉڍs
	if (mChangeTime > mSceneChangeTimer) {
		if (mCursorScript->IsCursorMove()) mSceneChangeTimer = 0.0f;
		if (!mCursorScript->IsChangeScene())mSceneChangeTimer += deltaTime;
	}
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
			if (mChangeTime <= mSceneChangeTimer) game->LoadScene("./Assets/Scenes/PVPlay.scene");
			else  mCursorScript->OnChangeScene();
		}
	}
	// �J�����̐e�̈ړ�
	camera->mTransform->Position(
		cameraPosition +
		(camera->mTransform->Left() * (mCameraRotateSpeed * 0.01f) * (deltaTime * 60.0f)));
	// + (camera->mTransform->Left() * (mCameraRotateSpeed * (3.14f * 2.0f)) * 0.01)
	//�J�����̐e����]������ -> �J�����{�̂���]�\�ɂ����邽�� 
	auto centerPosition = center->mTransform->Position();
	auto v = centerPosition - camera->mTransform->Position();
	auto angle = atan2(v.x, v.z);
	auto escapeAngle = angle;
	if (std::to_string(escapeAngle) == "0.000000") escapeAngle = 0.0f;
	if (escapeAngle < 0) escapeAngle += 3.141593f * 2.0f;

	camera->mTransform->Rotate(camera->mTransform->Up() * (escapeAngle));
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void TitleMnager::Finish(){
	game->DestroyObject(mFadeOutObj);
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void TitleMnager::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void TitleMnager::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void TitleMnager::OnCollideExit(Actor* target){
	(void)target;
}

// �^�C�g���̃f�t�H���g�̃{�^�����쐬���܂�
void TitleMnager::createButton(const int number, const int count) {
	//mSelectNumberContainer[selectNumber]
	/*auto stageSelectButtons = game->FindActor("StageSelectButtons");
	if (stageSelectButtons != nullptr) {
		deleteButtonObj("StageSelectButtons");
		auto titleObj = game->CreateActor("Assets/SceneAssets/Title_Buttons");
		game->AddObject(titleObj);
	}*/
	auto stageSelectButtons = game->FindActor(mSelectNumberContainer[number]);
	if (stageSelectButtons != nullptr) {
		deleteButtonObj(mSelectNumberContainer[number]);
		auto titleObj = game->CreateActor(mSelectNumberContainer[number + 1]);
		game->AddObject(titleObj);
	}
	for (auto i = 0; i != count; i++)
	{
		auto selectNumber = i + number + 2;
		// �{�^���̒ǉ�
		mCursorScript->AddButtonContainer(mSelectNumberContainer[selectNumber]);
		// �J�ڐ�̃V�[���̒ǉ�
		mCursorScript->AddSceneContainer(mSelectNumberContainer[selectNumber + count]);
	}
	if(mTitleButtonCount != 0) mCursorScript->SetPosition();
	mTitleButtonCount += 1;
	//mTitleButtonCount = 0;
}

// �{�^���I�u�W�F�N�g�̍폜
void TitleMnager::deleteButtonObj(const char* name) {
	mCursorScript->ResetButtonContainer();
	auto buttonObj = game->FindActor(name);
	game->DestroyObject(buttonObj);
}