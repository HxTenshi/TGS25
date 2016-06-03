#include "TextureScroll.h"
#include "Game/Actor.h"
#include "Game/Component/MaterialComponent.h"
#include "Graphic/Material/Material.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TextureScroll::Initialize(){
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TextureScroll::Start(){

}

//���t���[���Ă΂�܂�
void TextureScroll::Update(){
	auto mate = gameObject->GetComponent<MaterialComponent>();
	if (mate){
		auto material = mate->GetMaterialPtr(0);
		material->mCBMaterial.mParam.MOffset.x += x;
		material->mCBMaterial.mParam.MOffset.y += y;
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void TextureScroll::Finish(){
	
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void TextureScroll::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void TextureScroll::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void TextureScroll::OnCollideExit(Actor* target){
	(void)target;
}