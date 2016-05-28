#include "HPGauge.h"

#include "Input/Input.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"PhysX\IPhysXEngine.h"


#include"SailBoard.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void HPGauge::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void HPGauge::Start(){

}

//���t���[���Ă΂�܂�
void HPGauge::Update()
{
	
	//���� = (�c��HP / HP�ő�l) * �O�g�̉���
	auto playerScript = game->FindActor("Board")->GetScript<SailBoard>();
	if (playerScript)
	{
		auto height = (playerScript->GetHitPoint() / 100.0f) * 660;
		gameObject->mTransform->Scale(XMVectorSet(20, height, 1, 1));
		gameObject->mTransform->Position(XMVectorSet(1090, 740 - (height / 2), 1, 1));
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void HPGauge::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void HPGauge::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void HPGauge::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void HPGauge::OnCollideExit(Actor* target){
	(void)target;
}