#include "TitleMnager.h"
#include "Input/Input.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"PhysX\IPhysXEngine.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TitleMnager::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TitleMnager::Start(){

}

//���t���[���Ă΂�܂�
void TitleMnager::Update()
{
	if (Input::Trigger(PAD_DS4_KeyCoord::Button_CIRCLE) || Input::Trigger(KeyCoord::Key_J))
	{
		game->LoadScene("./Assets/Scenes/nishiWindSurfing.scene");
	}
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