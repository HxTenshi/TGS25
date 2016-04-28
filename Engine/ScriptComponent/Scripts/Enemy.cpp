#include "Enemy.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Engine\DebugEngine.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Initialize(){
	mSpeed = 4.0f;
	mSize = 4;
	mIsHit = false;
	mIsSearchRange = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Start(){
	//auto color = XMFLOAT4(0, 0, 0, 1);

	//auto mate = gameObject->GetComponent<MaterialComponent>();// ->SetAlbedoColor(color);
	//if (mate) {
	//	mate->SetAlbedoColor(color);
	//}
}

//���t���[���Ă΂�܂�
void Enemy::Update(){
	

	PlayerSearch();
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Finish(){

}

void Enemy::PlayerSearch() {
	// �F�̐ݒ�
	auto color = XMFLOAT4(1, 1, 1, 1);
	// ��苗�������ƐF�̒l��ς���
	if (mIsSearchRange == true) {
		color = XMFLOAT4(1, 0, 0, 1);
	}
	else {
		color = XMFLOAT4(1, 1, 1, 1);
	}
	// �F�̍X�V
	auto mate = gameObject->GetComponent<MaterialComponent>();// ->SetAlbedoColor(color);
	if (mate) {
		mate->SetAlbedoColor(color);
	}

	// �v���C���[�̑{��
	auto obj = game->FindActor("Board");
	if (!obj) return;

	auto target = ((obj->mTransform->Position()) - gameObject->mTransform->Position());
	auto targetRange = XMVector3Length(obj->mTransform->Position() - gameObject->mTransform->Position());

	target = XMVector3Normalize(target);
	//target.y = 0;

	// �ړ�
	if (mIsHit == true) {
		//gameObject->mTransform->AddForce(target * 0);
		gameObject->mTransform->Forward() * 0;
		//gameObject->mTransform->Position() -= target;
	}
	else {
		//game->Debug()->Log(std::to_string(targetRange.x));
		// ��苗���Ȃ甽������
		if (targetRange.x < mSize * 3) {
			gameObject->mTransform->AddForce(target*mSpeed);
			mIsSearchRange = true;
		}
		else if (targetRange.x >= mSize * 5) {
			mIsSearchRange = false;
		}
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Enemy::OnCollideBegin(Actor* target){
	(void)target;
	if (target->Name() == "Board"){
		mIsHit = true;
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Enemy::OnCollideEnter(Actor* target){
	(void)target;
	if (target->Name() == "Board") {
		mIsHit = true;
	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Enemy::OnCollideExit(Actor* target){
	(void)target;
	mIsHit = false;
}