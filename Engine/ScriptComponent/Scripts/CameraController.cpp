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
	mPrevJump = false;
	mTimer = 0;
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

		//�v���C���[���W�����v������J�����̈ʒu�ύX�̏���
		if (mPrevJump != mTarget->GetScript<SailBoard>()->GetIsJump())
		{
			mFromPos = mPosition;
			mTimer = 0;
			mPrevJump = mTarget->GetScript<SailBoard>()->GetIsJump();
		}

		if (!mTarget->GetScript<SailBoard>()->GetIsJump())
		{
			mPosition = Lerp(mFromPos, mTarget->mTransform->Position() + (mTarget->mTransform->Forward() * -13) + XMVectorSet(0, 3.5f, 0, 1));
			if (Input::Down(PAD_DS4_KeyCoord::Button_DOWN))
			{
				mPosition = Lerp(mFromPos, mTarget->mTransform->Position() + (mTarget->mTransform->Forward() * 3) + XMVectorSet(0, 1, 0, 1));
			}
		}
		else
		{
			mPosition = Lerp(mFromPos, mTarget->mTransform->Position() + XMVectorSet(0, 1, -5, 1));
		}
		gameObject->mTransform->Position(mPosition);
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
	auto vec = XMVectorSet(0, 0, 0, 1);
	if (!mTarget->GetScript<SailBoard>()->GetIsJump())
	{
		auto Angel2 = mTarget->mTransform->Rotate().x;
		vec = XMVectorSet(Angel2, Angel, 0, 1);
	}
	else
	{
		vec = XMVectorSet(0, Angel, 0, 1);
	}
	gameObject->mTransform->Rotate(vec);

}

XMVECTOR CameraController::Lerp(XMVECTOR p1, XMVECTOR p2)
{
	mTimer += 0.2f;
	mTimer = min(1, mTimer);
	return XMVectorLerp(p1, p2, mTimer);
}

