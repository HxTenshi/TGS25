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
		gameObject->mTransform->Position(mPlayer->mTransform->Position() + mPlayer->mTransform->Left() * 3 + XMVectorSet(0,2,0,0));
	}
	auto player = mPlayer->GetScript<SailBoard>();
	if (player) {
		auto windvec = player->GetWind();
		if (XMVector3Length(windvec).x > 0)
		{

			auto temp = XMVectorSet(1, 0, 0, 1);

			temp = XMVector3Normalize(temp);
			windvec = XMVector3Normalize(windvec);

			auto dot = XMVector3Dot(temp, windvec);
			auto radian = acos(dot.x);
			auto rotate = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), radian);
			gameObject->mTransform->Quaternion(rotate);

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