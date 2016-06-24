#include "Tornado.h"
#include"h_component.h"
#include"h_standard.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Tornado::Initialize(){
	mTimer = 0.0f;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Tornado::Start()
{
	mScale = gameObject->mTransform->Scale();
}

//���t���[���Ă΂�܂�
void Tornado::Update()
{
	auto mate = gameObject->GetComponent<MaterialComponent>();
	if (mate) {
		auto material = mate->GetMaterialPtr(0);
		material->mCBMaterial.mParam.MOffset.x += x;
		material->mCBMaterial.mParam.MOffset.y += y;
	}

	if (mTimer > LimitTime)
	{
		mScale -= XMVectorSet(0.01,0.01,0.01,0);
		gameObject->mTransform->Scale(mScale);
		if(mScale.x < 0) game->DestroyObject(gameObject);
	}

	mTimer += game->DeltaTime()->GetDeltaTime();

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Tornado::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Tornado::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Tornado::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Tornado::OnCollideExit(Actor* target){
	(void)target;
}