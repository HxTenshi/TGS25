#include "HaneEffect.h"
#include"h_component.h"
#include"h_standard.h"
#include"PlayerManager.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void HaneEffect::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void HaneEffect::Start(){
	mScale = gameObject->mTransform->Scale();
	isEnd = false;
}

//���t���[���Ă΂�܂�
void HaneEffect::Update()
{
	if (isEnd)
	{
		mScale -= (XMVectorSet(1, 1, 0, 0) * 20);
		gameObject->mTransform->Scale(mScale);
		if (mScale.x < 100)
		{
			auto manager = game->FindActor("PlayerManager")->GetScript<PlayerManager>();
			manager->ItemGet();
			game->DestroyObject(gameObject);
		}
	}
	else
	{
		mPosition = XMVectorLerp(gameObject->mTransform->Position(), mSetPosition, mTimer);
		gameObject->mTransform->Position(mPosition);
		mTimer += game->DeltaTime()->GetNoScaleDeltaTime() * 0.5f;

		if (mTimer >= 1)
		{
			mScale += (XMVectorSet(1, 1, 0, 0) * 20);
			if (mScale.x > 200)
			{
				isEnd = true;
			}
			gameObject->mTransform->Scale(mScale);
		}
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void HaneEffect::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void HaneEffect::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void HaneEffect::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void HaneEffect::OnCollideExit(Actor* target){
	(void)target;
}

void HaneEffect::SetPosition(XMVECTOR pos)
{
	mSetPosition = pos;
}
