#include "Squid.h"
#include "Enemy.h"
#include "PlayerSearch.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Squid::Initialize(){
	Enemy::Initialize();

	mInitMoveInterval = mMoveInterval;
	mInitWaitInterval = mWaitInterval;

	mCreatCount = 0;
	mSRPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Squid::Start(){
	Enemy::Start();
	Enemy::setDamage(mSetDamage);
}

//���t���[���Ă΂�܂�
void Squid::Update(){
	auto parentObj = gameObject->mTransform->GetParent();
	//auto p = parentObj->GetComponent<TransformComponent>();

	auto objPosition = parentObj->mTransform->Position();
	auto parentScale = parentObj->mTransform->Scale();
	//auto forwardMove = parentObj->mTransform->Forward() * 4.0f;

	//auto squidObj = game->FindActor("SquidParent");

	if (mCreatCount == 0) {
		//Enemy::PlayerSearchMode(parentObj);
		mPlayerSearchObj = game->CreateActor("Assets/PlayerSearchRange");
		game->AddObject(mPlayerSearchObj);

		// ���G�I�u�W�F�̃X�N���v�g�擾
		searchScript = mPlayerSearchObj->GetScript<PlayerSearch>();
		// �傫���̕ύX
		auto objScale = gameObject->mTransform->Scale();
		auto setScale = XMVectorSet(objScale.x, objScale.y * 2.0f, objScale.z * 3.0f, 0.0f);
		mPlayerSearchObj->mTransform->Scale(setScale);

		// ���G�͈̓I�u�W�F�̈ʒu�ݒ�
		auto childrenScale = mPlayerSearchObj->mTransform->Scale();
		// �ʒu�̎w��
		auto setPosition = XMVectorSet(0.0f, 0.0f, (objScale.z + childrenScale.z) / 2.0f, 0.0f);
		mSRPosition = setPosition;

		// �ʒu�̕ύX
		mPlayerSearchObj->mTransform->Position(-setPosition);
		game->Debug()->Log(std::to_string(mPlayerSearchObj->mTransform->Position().z));

		// �G�̎q���ɒǉ�����
		mPlayerSearchObj->mTransform->SetParent(parentObj);

		mCreatCount = 1;
	}

	if (searchScript->IsPlayerSearch()) {
		mIsSearchRange = true;
		//game->Debug()->Log("������");
	}
	else {
		mIsSearchRange = false;
	}

	//game->Debug()->Log(std::to_string(mPlayerSearchObj->mTransform->Position().z));

	// ���G�I�u�W�F�̈ʒu�X�V
	mPlayerSearchObj->mTransform->Position(-mSRPosition);

	if (parentObj->Name() == "SquidParent") {
		ParentMove();

		//auto squidPosition = squidObj->mTransform->Position();		

		/*auto pRotate = parentObj->mTransform->Rotate();
		auto pRotateY = 3.141593f / 180.0f;
		auto rotateV = XMVectorSet(0.0f, pRotateY, 0.0f, 0.0f);
		parentObj->mTransform->Rotate(pRotate + rotateV);*/

		// �v���C���[�̕����������i�܂��j
		if (mIsSearchRange) {
			auto playerObj = game->FindActor("Board");
			auto playPo = playerObj->mTransform->Position();

			auto parPo = parentObj->mTransform->Position();
			auto parRo = parentObj->mTransform->Rotate();
			auto YzeroPlayerPosi = XMVectorSet(playPo.x, parPo.y, playPo.z, 0.0f);
			//auto parAngle = parRo.y * 57.3f;
			auto setForPo = XMVectorSet(parPo.x - (20.0f * sinf(parRo.y)),
				parPo.y, parPo.z - (20.0f * cosf(parRo.y)), 0.0f);

			auto pRotate = parentObj->mTransform->Rotate();
			auto pRotateY = 3.141593f / 180.0f / 10.0f;
			auto rotateV = XMVectorSet(0.0f, pRotateY, 0.0f, 0.0f);
			auto distance = XMVector3LinePointDistance(
				parPo,
				setForPo,
				YzeroPlayerPosi);

			auto nomalizeSetForPosi = XMVector3Normalize(setForPo);
			auto nomalizeYZERO = XMVector3Normalize(YzeroPlayerPosi);

			auto nomalizeAdd = (nomalizeSetForPosi.x * nomalizeYZERO.x)
				+ (nomalizeSetForPosi.z * nomalizeYZERO.z);

			//game->Debug()->Log(std::to_string(sinf(pRotate.y)));
			//game->Debug()->Log(std::to_string(distance.y));
			//game->Debug()->Log(std::to_string(setForPo.x - playPo.x));
			game->Debug()->Log(std::to_string(cosf(nomalizeAdd)));
			//game->Debug()->Log(std::to_string(parPo.z - (20.0f * cosf(parRo.y))));

			// �l�̉��Z
			/*if (distance.y > 0.01f) {

				if (setForPo.x - playPo.x < 0) {
					parentObj->mTransform->Rotate(pRotate - rotateV);
				}
				else if (setForPo.x - playPo.x > 0) {
					parentObj->mTransform->Rotate(pRotate + rotateV);
				}
			}*/
			
		}
	}
	else {
		game->Debug()->Log("������Ȃ�");
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Squid::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Squid::OnCollideBegin(Actor* target){
	Enemy::OnCollideBegin(target);
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Squid::OnCollideEnter(Actor* target){
	Enemy::OnCollideEnter(target);
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Squid::OnCollideExit(Actor* target){
	Enemy::OnCollideExit(target);
}

void Squid::ParentMove() {
	// �e�𓮂���
	auto parentPosition = mParentObj->mTransform->Position();
	auto forwardMove = mParentObj->mTransform->Forward() * (mSpeed * 0.01f);

	if (mMoveInterval > 0) {
		mParentObj->mTransform->Position(parentPosition + forwardMove);
		mMoveInterval--;
	}
	else {
		if (mWaitInterval > 0) {
			mWaitInterval--;
		}
		else {
			mMoveInterval = mInitMoveInterval;
			mWaitInterval = mInitWaitInterval;
		}
	}
}