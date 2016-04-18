#include "WindArrow.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"
#include "Wind.h"



//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WindArrow::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WindArrow::Start(){

}

//���t���[���Ă΂�܂�
void WindArrow::Update(){

	auto wind = game->FindActor("Wind");
	if (wind){
		auto windscr = wind->GetScript<Wind>();
		if (windscr){
			auto v = windscr->GetWindVelocity();
			if (XMVector3Length(v).x > 0){

				auto f = gameObject->mTransform->GetParent()->mTransform->Forward();
				f.y = 0;
				f = XMVector3Normalize(f);
				v = XMVector3Normalize(v);
				auto rpy = XMVector3AngleBetweenNormals(f, v);
				gameObject->mTransform->Rotate(XMVectorSet(0, rpy.x, 0, 1));

			}
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