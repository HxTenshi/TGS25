#include "PlayerSearch.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXColliderComponent.h"
#include "Engine\DebugEngine.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PlayerSearch::Initialize(){
	mPlayerDistance = 0.0f;
	mIsPlayerHit = false;
	mIsWallHit = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PlayerSearch::Start(){
	mSizeZ = gameObject->mTransform->Scale().z;
	auto collider = gameObject->GetComponent<PhysXColliderComponent>();
	collider->SetScale(gameObject->mTransform->Scale());
	//game->Debug()->Log(std::to_string(mSizeZ));
}

//���t���[���Ă΂�܂�
void PlayerSearch::Update(){
	// �v���C���[�̑{��
	auto object = game->FindActor("Board");
	if(!object) mIsPlayerHit = false;

	if (mIsPlayerHit) {
		// �e�̎擾
		auto parentObj = gameObject->mTransform->GetParent();
		auto parentPosition = parentObj->mTransform->Position();
		auto parentRotate = parentObj->mTransform->Rotate();

		// �e�̓����Ƀx�N�g����ݒ肷��
		auto parentHeadPoint = XMVectorSet(
			parentPosition.x - ((mSizeZ / mScalarZ) / 2.0f * sinf(parentRotate.y)),
			parentPosition.y,
			parentPosition.z - ((mSizeZ / mScalarZ) / 2.0f * cosf(parentRotate.y)),
			0.0f);
		
		// �v���C���[�Ɛe�̓����̋����̌v�Z
		auto targetRange = XMVector3Length(object->mTransform->Position()- parentHeadPoint);

		mPlayerDistance = targetRange.z;
		//game->Debug()->Log(std::to_string(targetRange.z));

		if (mPlayerDistance >= mSizeZ * (5.0f / 3.0f)) {
			//game->Debug()->Log("���Ȃ�");
			game->Debug()->Log(std::to_string(mPlayerDistance));
			game->Debug()->Log(std::to_string(mSizeZ * (5.0f / 3.0f)));
			mIsPlayerHit = false;
		}
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void PlayerSearch::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PlayerSearch::OnCollideBegin(Actor* target){
	if (target->Name() == "Wall") {
		wallObj = target;
		mIsWallHit = true;
	}

	if (target->Name() == "Board") {
		//game->Debug()->Log("������221e23131r2d");
		playerObj = target;
		// ����p���ǂƓ���������
		if (mIsWallHit) {
			auto playerLength = XMVector3Length(
				playerObj->mTransform->Position() - gameObject->mTransform->Position());
			// �v���C���[�ƓG�̊Ԃɕǂ�����������true
			if (!XMVector3InBounds(wallObj->mTransform->Position(), playerLength)) {
				mIsPlayerHit = true;
			}
		}
		else {
			mIsPlayerHit = true;
		}
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PlayerSearch::OnCollideEnter(Actor* target){
	if (target->Name() == "Wall") {
		wallObj = target;
		mIsWallHit = true;
	}

	if (target->Name() == "Board") {
		//game->Debug()->Log("������221e23131r2d");
		playerObj = target;
		// ����p���ǂƓ���������
		if (mIsWallHit) {
			auto playerLength = XMVector3Length(
				playerObj->mTransform->Position() - gameObject->mTransform->Position());
			// �v���C���[�ƓG�̊Ԃɕǂ�����������true
			if (!XMVector3InBounds(wallObj->mTransform->Position(), playerLength)) {
				mIsPlayerHit = true;
			}
		}
		else {
			mIsPlayerHit = true;
		}
	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void PlayerSearch::OnCollideExit(Actor* target){
	(void)target;
	if (target->Name() == "Wall") {
		wallObj = target;
		mIsWallHit = false;
	}
}

// �X�J���[�{�����l�̎擾
void PlayerSearch::SetScalarZ(const float scalarZ) {
	mScalarZ = scalarZ;
}

// �v���C���[�ƍ��G�͈͂̎n�_�Ƃ̋�����Ԃ��܂�
float PlayerSearch::PlayerDistance() {
	return mPlayerDistance;
}

// �v���C���[�ɓ�����������Ԃ��܂�
bool PlayerSearch::IsPlayerSearch() {
	return mIsPlayerHit;
}