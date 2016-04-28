#include "CameraController.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"
#include "Engine\DebugEngine.h"
#include<math.h>


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
	Look();

	mPosition = mTarget->mTransform->Position() + (mTarget->mTransform->Forward() * -3) + XMVectorSet(0,1,0,1);

	//mPosition = mTarget->mTransform->Position() + XMVectorSet(0, 1, -5, 0);
	gameObject->mTransform->Position(mPosition);
	//gameObject->mTransform->Rotate(XMQuaternionRotationMatrix(mat));
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

void CameraController::Look()
{
	auto SubVector = gameObject->mTransform->Position() - mTarget->mTransform->Position();

	auto Angel = atan2(-SubVector.x, -SubVector.z);
	if (Input::Down(KeyCoord::Key_F))
	game->Debug()->Log(std::to_string(Angel));

	auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), Angel);
	auto vec = XMVectorSet(0, Angel, 0, 1);
	gameObject->mTransform->Rotate(vec);

}
