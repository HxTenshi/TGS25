#include "WindArrow.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"
#include "Wind.h"
#include "SailBoard.h"
#include "Engine\DebugEngine.h"
#include<math.h>



//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WindArrow::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WindArrow::Start()
{
	mPlayer = game->FindActor("Board");
}

//���t���[���Ă΂�܂�
void WindArrow::Update(){

	if (mPlayer)
	{
		gameObject->mTransform->Position(mPlayer->mTransform->Position() + XMVectorSet(0.0f, 1.5f, 0.0f,1.0f));
	}
	auto wind = mPlayer->GetScript<SailBoard>();
	if (wind) {
		auto windvec = wind->GetWind();
		if (XMVector3Length(windvec).x > 0)
		{

			auto f = XMVectorSet(1, 0, 0, 1);
			f.y = 0;
			f = XMVector3Normalize(f);
			windvec = XMVector3Normalize(windvec);
			auto rpy = XMVector3Dot(f, windvec);
			auto r = acos(rpy.x);
			auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), r);
			gameObject->mTransform->Quaternion(rotatey);

		}
	}
	
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void WindArrow::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WindArrow::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WindArrow::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void WindArrow::OnCollideExit(Actor* target){
	(void)target;
}