#include "PlayerSearch.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Engine\DebugEngine.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PlayerSearch::Initialize(){
	mIsPlayerHit = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PlayerSearch::Start(){
	mSizeZ = gameObject->mTransform->Scale().z;
	game->Debug()->Log(std::to_string(mSizeZ));
}

//���t���[���Ă΂�܂�
void PlayerSearch::Update(){
	if (mIsPlayerHit) {
		// �v���C���[�̑{��
		auto object = game->FindActor("Board");
		if (!object) return;

		// �v���C���[�Ƃ̋����̌v�Z
		auto targetRange = XMVector3Length(
			object->mTransform->Position() - gameObject->mTransform->Position());

		// �e�̎擾
		auto parentObj = gameObject->mTransform->GetParent();
		auto parentSize = parentObj->mTransform->Scale();

		/*if (targetRange.z > mSizeZ * (5 / 3)) {
			game->Debug()->Log("hit");
			mIsPlayerHit = false;
		}*/
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void PlayerSearch::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PlayerSearch::OnCollideBegin(Actor* target){
	if (target->Name() == "Board") {
		game->Debug()->Log("������221e23131r2d");
		mIsPlayerHit = true;
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PlayerSearch::OnCollideEnter(Actor* target){

	if (target->Name() == "Board") {
		game->Debug()->Log("������d12d12fd1");
		mIsPlayerHit = true;
	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void PlayerSearch::OnCollideExit(Actor* target){
	(void)target;
}

bool PlayerSearch::IsPlayerSearch() {
	return mIsPlayerHit;
}