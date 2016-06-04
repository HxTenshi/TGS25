#include "PointItem.h"

#include "Input/Input.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"PhysX\IPhysXEngine.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PointItem::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PointItem::Start(){

}

//���t���[���Ă΂�܂�
void PointItem::Update()
{
	auto player = game->FindActor("Board");
	auto v = player->mTransform->Position() - gameObject->mTransform->Position();
	if (XMVector3Length(v).x < 5)
	{
		v = XMVector3Normalize(v);
		gameObject->mTransform->Position(gameObject->mTransform->Position() + (v * 0.1f));
	}

	mAngel += 0.01f;
	gameObject->mTransform->Quaternion(XMQuaternionRotationAxis(gameObject->mTransform->Up(), mAngel));
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void PointItem::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PointItem::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PointItem::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void PointItem::OnCollideExit(Actor* target){
	(void)target;
}