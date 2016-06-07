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

	mChaseStopDistance = mSizeZ * (5.0f / 3.0f);
	//game->Debug()->Log(std::to_string(mSizeZ));
}

//���t���[���Ă΂�܂�
void PlayerSearch::Update(){
	// �v���C���[�̑{��
	auto playerObj = game->FindActor("Board");
	if(!playerObj) mIsPlayerHit = false;

	//game->Debug()->Log(std::to_string(mIsPlayerHit));

	if (mIsPlayerHit) {

		PlayerDistance(playerObj);

		/*game->Debug()->Log("�G�̍��G�͈͊O����:" + std::to_string(mChaseStopDistance));
		game->Debug()->Log("�v���C���[�Ƃ̋���:" + std::to_string(mPlayerDistance));*/

		// mPlayerDistance

		//game->Debug()->Log(std::to_string(mChaseStopDistance));
		
		//// �e�̎擾
		//auto parentObj = gameObject->mTransform->GetParent();
		//auto parentPosition = parentObj->mTransform->Position();
		//auto parentRotate = parentObj->mTransform->Rotate();

		//// �e�̓����Ƀx�N�g����ݒ肷��
		//auto parentHeadPoint = XMVectorSet(
		//	parentPosition.x - ((mSizeZ / mScalarZ) / 2.0f * sinf(parentRotate.y)),
		//	parentPosition.y,
		//	parentPosition.z - ((mSizeZ / mScalarZ) / 2.0f * cosf(parentRotate.y)),
		//	0.0f);
		//
		//// �v���C���[�Ɛe�̓����̋����̌v�Z
		//auto targetRange = XMVector3Length(object->mTransform->Position()- parentHeadPoint);

		//mPlayerDistance = targetRange.z;
		////game->Debug()->Log(std::to_string(targetRange.z));

		if (mPlayerDistance >= mChaseStopDistance) {
			game->Debug()->Log("�G�̍��G�͈͊O����:" + std::to_string(mChaseStopDistance));
			game->Debug()->Log("�v���C���[�Ƃ̋���:" + std::to_string(mPlayerDistance));
			mIsPlayerHit = false;
		}
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void PlayerSearch::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PlayerSearch::OnCollideBegin(Actor* target){
	//if (target->Name() == "Wall") {
	//	wallObj = target;
	//	mIsWallHit = true;
	//}

	//if (target->Name() == "Board") {
	//	playerObj = target;
	//	PlayerDistance(target);

	//	// ����p���ǂƓ���������
	//	if (mIsWallHit) {
	//		auto playerLength = XMVector3Length(
	//			playerObj->mTransform->Position() - gameObject->mTransform->Position());
	//		// �v���C���[�ƓG�̊Ԃɕǂ�����������true
	//		if (!XMVector3InBounds(wallObj->mTransform->Position(), playerLength)) {
	//			mIsPlayerHit = true;
	//		}
	//	}
	//	else {
	//		mIsPlayerHit = true;
	//	}
	//}

	if (target->Name() == "Board") {
		PlayerDistance(target);
		mIsPlayerHit = true;
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PlayerSearch::OnCollideEnter(Actor* target){
	//if (target->Name() == "Wall") {
	//	wallObj = target;
	//	mIsWallHit = true;
	//}

	//if (target->Name() == "Board") {
	//	playerObj = target;
	//	// ����p���ǂƓ���������
	//	if (mIsWallHit) {
	//		auto playerLength = XMVector3Length(
	//			playerObj->mTransform->Position() - gameObject->mTransform->Position());
	//		// �v���C���[�ƓG�̊Ԃɕǂ�����������true
	//		if (!XMVector3InBounds(wallObj->mTransform->Position(), playerLength)) {
	//			mIsPlayerHit = true;
	//		}
	//	}
	//	else {
	//		mIsPlayerHit = true;
	//	}
	//}

	if (target->Name() == "Board") {
		mIsPlayerHit = true;
	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void PlayerSearch::OnCollideExit(Actor* target){
	(void)target;
	/*if (target->Name() == "Wall") {
		wallObj = target;
		mIsWallHit = false;
	}*/
}

// �X�J���[�{�����l�̎擾
void PlayerSearch::SetScalarZ(const float scalarZ) {
	mScalarZ = scalarZ;
}
// �v���C���[�Ƃ̋������v�Z���܂�
void PlayerSearch::PlayerDistance(Actor* playerObj) {
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
	auto targetRange = XMVector3Length(playerObj->mTransform->Position() - parentHeadPoint);

	mPlayerDistance = targetRange.z;
}

// �v���C���[�ƍ��G�͈͂̎n�_�Ƃ̋�����Ԃ��܂�
float PlayerSearch::GetPlayerDistance() {
	return mPlayerDistance;
}

// �v���C���[�ɓ�����������Ԃ��܂�
bool PlayerSearch::IsPlayerSearch() {
	return mIsPlayerHit;
}
// �v���C���[��ǐՒ��~���鋗���ɉ��Z���܂�
void PlayerSearch::AddChaseStopDistance(float distance) {
	mChaseStopDistance += distance;

	/*game->Debug()->Log("���Z");
	game->Debug()->Log(std::to_string(mChaseStopDistance));*/

}