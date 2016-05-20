#include "Enemy.h"
#include "PlayerSearch.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Engine\DebugEngine.h"

Enemy::~Enemy() {
	//gameObject->mTransform->AllChildrenDestroy();
}

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Initialize(){
	mAddSearchObjCount = 0;
	mSpeed = 1.0f * 0.01f;
	mSize = gameObject->mTransform->Scale();
	mHalfSizeZ = mSize.z / 2.0f;
	mPositionY = gameObject->mTransform->Position().y;
	mRotateY = 0.0f;
	mDeadHeigth = -10.0f;
	mIsObjHit = false;
	mIsFloorHit = false;
	mIsSearchRange = false;
	mIsImmortalBody = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Start(){
	//game->Debug()->Log(std::to_string(mSize));

	auto nowPosition = gameObject->mTransform->Position();
	mPositionY = nowPosition.y;
}

//���t���[���Ă΂�܂�
void Enemy::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Finish(){
	/*game->DestroyObject(mPlayerSearchObj);*/
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Enemy::OnCollideBegin(Actor* target){
	if (target->Name() == "Board"){
		if (!mIsSearchRange && !mIsImmortalBody) {
			game->DestroyObject(gameObject);
		}
		mIsObjHit = true;
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Enemy::OnCollideEnter(Actor* target){
	if (target->Name() == "Board") {
		mIsObjHit = true;
	}

	if (target->Name() == "Floor") {
		mIsFloorHit = true;
	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Enemy::OnCollideExit(Actor* target){
	if (target->Name() == "Board") {
		mIsObjHit = false;
	}

	if (target->Name() == "Floor") {
		mIsFloorHit = false;
	}
}

void Enemy::PlayerColorChange(Actor* obj) {
	// �F�̐ݒ�
	auto color = XMFLOAT4(1, 1, 1, 1);
	// ��苗�������ƐF�̒l��ς���
	if (mIsSearchRange) {
		color = XMFLOAT4(1, 0, 0, 1);
	}
	else {
		color = XMFLOAT4(1, 1, 1, 1);
	}
	// �F�̍X�V
	auto mate = gameObject->GetComponent<MaterialComponent>();
	if (mate) {
		//game->Debug()->Log(std::to_string(color.y));
		mate->SetAlbedoColor(color);
	}
}

void Enemy::SearchMove() {
	auto v = gameObject->mTransform->Forward() * mSpeed;
	auto position = gameObject->mTransform->Position();
	gameObject->mTransform->Position(position + v);

	mRotateY += 3.14 / 180;
	
	auto rotate = XMVectorSet(0.0f, mRotateY, 0.0f, 0.0f);

	gameObject->mTransform->Rotate(rotate);
}

void Enemy::PlayerSearchMode(Actor* thisObj, const float sizeX, const float sizeY, const float sizeZ) {
	// �v���C���[�̑{��
	/*auto object = game->FindActor("Board");
	if (!object) return;*/

	// ���G�͈̓I�u�W�F�̍쐬
	if (mAddSearchObjCount < 1) {
		mPlayerSearchObj = game->CreateActor("Assets/PlayerSearchRange");
		game->AddObject(mPlayerSearchObj);
		// �G�̎q���ɒǉ�����
		mPlayerSearchObj->mTransform->SetParent(gameObject);
		// ���G�I�u�W�F�̃X�N���v�g�擾
		mPlayerSearchObj->GetScript<PlayerSearch>();
		// �傫���̕ύX
		auto objScale = gameObject->mTransform->Scale();
		auto setScale = XMVectorSet(sizeX, sizeY, sizeZ, 0.0f);
		mPlayerSearchObj->mTransform->Scale(setScale);
		// �ڂ̑O�̈ʒu�ɐݒu����
		Enemy::SetForwardChildrenObj(mPlayerSearchObj);

		// ��x����������ēx�������Ȃ�
		mAddSearchObjCount = 1;
	}

	searchScript = mPlayerSearchObj->GetScript<PlayerSearch>();

	if (searchScript->IsPlayerSearch()) {
		mIsSearchRange = true;
		//game->Debug()->Log("������");
	}
	else {
		mIsSearchRange = false;
	}
}

void Enemy::PlayerSearchMode(Actor* thisObj) {
	// ���G�͈̓I�u�W�F�̍쐬
	if (mAddSearchObjCount < 1) {
		mPlayerSearchObj = game->CreateActor("Assets/PlayerSearchRange");
		game->AddObject(mPlayerSearchObj);
		// �G�̎q���ɒǉ�����
		mPlayerSearchObj->mTransform->SetParent(gameObject);
		// ���G�I�u�W�F�̃X�N���v�g�擾
		searchScript = mPlayerSearchObj->GetScript<PlayerSearch>();
		// �傫���̕ύX
		auto objScale = gameObject->mTransform->Scale();
		auto setScale = XMVectorSet(objScale.x, objScale.y * 2.0f, 3.0f, 0.0f);
		mPlayerSearchObj->mTransform->Scale(setScale);
		// �ڂ̑O�̈ʒu�ɐݒu����
		Enemy::SetForwardChildrenObj(mPlayerSearchObj);

		// ��x����������ēx�������Ȃ�
		mAddSearchObjCount = 1;
	}

	//searchScript = mPlayerSearchObj->GetScript<PlayerSearch>();


	if (searchScript->IsPlayerSearch()) {
		mIsSearchRange = true;
		//game->Debug()->Log("������");
	}
	else {
		mIsSearchRange = false;
	}
}

void  Enemy::PlayerChaseMode(Actor* thisObj) {
	// �v���C���[�̑{��
	auto object = game->FindActor("Board");
	if (!object) {
		return;
	}
	else {
		// �v���C���[�Ƃ̋����̌v�Z
		auto target = ((object->mTransform->Position()) - thisObj->mTransform->Position());
		// ���K��
		target = XMVector3Normalize(target);

		// �ړ�
		if (mIsObjHit == false) {

			if (mIsSearchRange) {
				//// �v���C���[�̕���������
				auto mRotate = XMVectorSet(target.x, 0.0f, target.z, 0.0f);
				
				gameObject->mTransform->Quaternion(mRotate);

				//thisObj->mTransform->Quaternion(target);
				/*auto rotateZero = XMVectorSet(0.0f, gameObject->mTransform->Rotate().y, 0.0f, 0.0f);
				gameObject->mTransform->Rotate(rotateZero);*/

				/*auto thisObj = gameObject->mTransform->Position();
				auto player = object->mTransform->Position();
				auto addVector = thisObj + player;
				auto rotateY = 180.0f / 3.14f;
				auto objRotate = XMVectorSet(0.0f, rotateY, 0.0f, 0.0f);
				if (addVector.x < 0) {
					gameObject->mTransform->Rotate(gameObject->mTransform->Rotate() + -objRotate);
				}
				else if (addVector.x > 0) {
					gameObject->mTransform->Rotate(gameObject->mTransform->Rotate() + objRotate);
				}*/
			}

		}
	}
}

void Enemy::PlayerChase(Actor* thisObj) {
	// �ړ�
	if (!mIsObjHit) {
		auto moveSpeed = thisObj->mTransform->Forward() * -mSpeed;
		auto movePosition = thisObj->mTransform->Position();
		// Y���̕␳ (�G�̈ꕔ���C�ʂɏo�Ă���悤�ɂ���)
		if (mIsFloorHit) {
			auto setPosition = XMVectorSet(movePosition.x, mPositionY, movePosition.z, 0.0f);
			gameObject->mTransform->Position(setPosition + moveSpeed);
		}
		else {
			thisObj->mTransform->Position(movePosition + moveSpeed);
		}
	}
}

// �Ԋu�ړ�
void Enemy::PlayerSpaceChase(Actor* thisObj, const int interval, int count) {
	// �v���C���[�̑{��
	auto object = game->FindActor("Board");
	if (!object) return;

	auto target = ((object->mTransform->Position()) - thisObj->mTransform->Position());
	auto targetRange = XMVector3Length(object->mTransform->Position() - thisObj->mTransform->Position());

	target = XMVector3Normalize(target);

	//target.y = 0;

	// �ړ�
	if (!mIsObjHit) {
		// ��苗���Ȃ甽������
		if (targetRange.x <= (mSize.z / 2) + (mSize.z * 6)) {
			if (count % interval < interval / 2) {
				gameObject->mTransform->AddForce(target*mSpeed);
			}
			mIsSearchRange = true;
		}
		else if (targetRange.x >(mSize.z / 2) + (mSize.z * 6)) {
			gameObject->mTransform->Forward() * 0;
			mIsSearchRange = false;
		}
	}
}

// �ڂ̑O�ɃI�u�W�F�𐶐����܂�
void Enemy::SetForwardObj(Actor* setObj) {
	auto objPosition = gameObject->mTransform->Position();
	auto objScale = gameObject->mTransform->Scale();
	auto setObjScale = setObj->mTransform->Scale();
	// �ʒu�̎w��
	auto setPosition = XMVectorSet(0.0f, 0.0f, ((1.0f - objScale.z) / 2.0f) + (objScale.z + setObjScale.z) / 2.0f, 0.0f);
	// �ʒu�̕ύX
	setObj->mTransform->Position(objPosition + -setPosition);
	// �������̃I�u�W�F�̉�]�p�ɕύX
	setObj->mTransform->Rotate(gameObject->mTransform->Rotate());
}

// �e�̃I�u�W�F�̑O�Ɏq�̃I�u�W�F���쐬���܂�
void Enemy::SetForwardChildrenObj(Actor* setObj) {
	// ���G�͈̓I�u�W�F�̈ʒu�ݒ�
	auto objScale = gameObject->mTransform->Scale();
	auto childrenScale = setObj->mTransform->Scale();
	// �ʒu�̎w��
	auto setPosition = XMVectorSet(0.0f, 0.0f, ((1.0f - objScale.z) / 2.0f) + (objScale.z + childrenScale.z) / 2.0f, 0.0f);
	// �ʒu�̕ύX
	setObj->mTransform->Position(-setPosition);
}

// ��苗���܂ŗ������玀�S���܂�
void Enemy::FallDead(Actor* thisObj) {
	// ���̈ʒu�܂ŗ������玀�S
	if (thisObj->mTransform->Position().y <= mDeadHeigth) {
		game->DestroyObject(thisObj);
	}
}

// �v���C���[�Ƃ̋����̌v�Z (Z���W)
float Enemy::PlayerDistance() {
	// �v���C���[�̌���
	auto object = game->FindActor("Board");
	//if (!object) return 0.0f;
	// �v���C���[�Ƃ̋����̌v�Z
	auto targetRange = XMVector3Length(
		object->mTransform->Position() - gameObject->mTransform->Position());
	return targetRange.z;
}