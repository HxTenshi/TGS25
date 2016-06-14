#include "TitleMnager.h"
#include "Input/Input.h"
//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"PhysX\IPhysXEngine.h"

#include "SceneCursor.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TitleMnager::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TitleMnager::Start(){
	// �J�[�\���̎擾
	mCursorObj = game->FindActor("SceneCursor");
	mCursorScript = mCursorObj->GetScript<SceneCursor>();
	// �{�^���̒ǉ�
	mCursorScript->AddButtonContainer("GameStart_Button");
	mCursorScript->AddButtonContainer("Credit_Button");
	mCursorScript->AddButtonContainer("How_To_Button");
	mCursorScript->AddButtonContainer("GameEnd_Button");
	// �V�[���̒ǉ�
	mCursorScript->AddSceneContainer("Stage00");
	mCursorScript->AddSceneContainer("Stage00");
	mCursorScript->AddSceneContainer("Stage00");
	mCursorScript->AddSceneContainer("Stage00");
}

//���t���[���Ă΂�܂�
void TitleMnager::Update()
{
	auto camera = game->FindActor("MainCamera");
	auto cameraPosition = camera->mTransform->Position();
	auto cameraRotate = camera->mTransform->Rotate();
	// �f���^�^�C���̎擾
	auto deltaTime = game->DeltaTime()->GetDeltaTime();


	cameraRotate.y += cameraRotateSpeed * (3.14f / 180.0f) * (deltaTime * 60.0f);
	if (cameraRotate.y >= 3.14f * 2.0f) cameraRotate.y -= 3.14f * 2.0f;
	if (cameraRotate.y < 0.0f) cameraRotate.y += 3.14f * 2.0f;

	/*auto cameraPo = XMVectorSet(
		cameraPosition.x * sinf(cameraRotate.y), 0.0f,
		cameraPosition.z * cosf(cameraRotate.y), 0.0f);
	camera->mTransform->Position(cameraPo * 10.0f);*/
	camera->mTransform->Rotate(cameraRotate);

	//// �J�����̉�]
	//cameraRotate.y += cameraRotateSpeed * (3.14f / 180.0f) * (deltaTime * 60.0f);
	//if (cameraRotate.y >= 3.14f * 2.0f) cameraRotate.y -= 3.14f * 2.0f;
	//if (cameraRotate.y < 0.0f) cameraRotate.y += 3.14f * 2.0f;
	//camera->mTransform->Rotate(cameraRotate);
	// �܂��t�F�C�h�A�E�g�Ȃ��ł�
	if(mCursorScript->IsChangeScene()) mCursorScript->OnButtonScene();

	/*if (Input::Trigger(PAD_DS4_KeyCoord::Button_CIRCLE) || Input::Trigger(KeyCoord::Key_J))
	{
		game->LoadScene("./Assets/Scenes/nishiWindSurfing.scene");
	}*/
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void TitleMnager::Finish(){

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