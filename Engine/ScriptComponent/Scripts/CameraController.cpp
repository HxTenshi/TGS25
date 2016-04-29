#include "CameraController.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"SailBoard.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void CameraController::Initialize(){
	

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void CameraController::Start(){
	mTarget = game->FindActor("Board");
	gameObject->mTransform->Position(XMVectorSet(0, 0, 0, 1));
}

//���t���[���Ă΂�܂�
void CameraController::Update()
{
	if (mTarget)
	{
		Look();
		if (!mTarget->GetScript<SailBoard>()->GetIsJump())
		{
			mPosition = mTarget->mTransform->Position() + (mTarget->mTransform->Forward() * -3) + XMVectorSet(0, 1, 0, 1);
		}
		else
		{
			mPosition = mTarget->mTransform->Position() + XMVectorSet(0, 1, -5, 1);
		}
		//mPosition = mTarget->mTransform->Position() + XMVectorSet(0, 1, -5, 0);
		gameObject->mTransform->Position(mPosition);
		//gameObject->mTransform->Rotate(XMQuaternionRotationMatrix(mat));
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void CameraController::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void CameraController::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void CameraController::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void CameraController::OnCollideExit(Actor* target){
	(void)target;
}

//�v���C���[������
void CameraController::Look()
{
	auto SubVector = gameObject->mTransform->Position() - mTarget->mTransform->Position();

	auto Angel = atan2(-SubVector.x, -SubVector.z);

	auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), Angel);
	auto vec = XMVectorSet(0, Angel, 0, 1);
	gameObject->mTransform->Rotate(vec);

}
