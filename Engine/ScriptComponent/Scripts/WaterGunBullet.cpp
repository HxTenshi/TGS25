#include "WaterGunBullet.h"
#include "SailBoard.h"

//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"
#include"CCBoard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WaterGunBullet::Initialize(){
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WaterGunBullet::Start(){
}

//���t���[���Ă΂�܂�
void WaterGunBullet::Update(){
	// �O���Ɉړ�
	auto deltaTime = game->DeltaTime()->GetDeltaTime();
	mForwardVelocity = gameObject->mTransform->Forward() * -mSpeed * 0.01f;

	auto position = gameObject->mTransform->Position();
	gameObject->mTransform->Position(position + mForwardVelocity * (deltaTime * 60.0f));

	mDestroyTime -= deltaTime * 60.0f;
	if (mDestroyTime <= 0) {
		game->DestroyObject(gameObject);
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void WaterGunBullet::Finish(){
	/*gameObject->mTransform->AllChildrenDestroy();
	game->DestroyObject(gameObject);*/
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WaterGunBullet::OnCollideBegin(Actor* target){
	// �v���C���[�ɓ������������
	if (target->Name() == "Board") {
		auto playerScript = target->GetScript<CCBoard>();
		playerScript->Damage(mSetDamege);
		//// �T�E���h��炷
		/*auto sound = gameObject->GetComponent<SoundComponent>();
		if (!sound) return;
		sound->LoadFile("Assets/Enemy/hit.wav");
		sound->Play();*/
		game->DestroyObject(gameObject);
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WaterGunBullet::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void WaterGunBullet::OnCollideExit(Actor* target){
	(void)target;
}