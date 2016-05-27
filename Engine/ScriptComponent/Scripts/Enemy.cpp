#include "Enemy.h"
#include "PlayerSearch.h"
#include "SailBoard.h"

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
	mEnemyState = EnemyState::PlayerSearch;
	mAddSearchObjCount = 0;
	mDamage = 0;
	mSpeed = 1.0f * 0.01f;
	mSize = gameObject->mTransform->Scale();
	mHalfSizeZ = mSize.z / 2.0f;
	mPositionY = gameObject->mTransform->Position().y;
	mRotateY = 0.0f;
	mScalarX = 1.0f;
	mScalarY = 2.0f;
	mScalarZ = 3.0f;
	//mDeadHeigth = -10.0f;
	mIsFloorHit = false;
	mIsImmortalBody = false;
	mIsDistanceAct = false;
	mIsAttckMode = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Start(){
	// �e�̎擾	
	mParentObj = gameObject->mTransform->GetParent();
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
		auto playerScript = target->GetComponent<SailBoard>();
		// �v���C���[�����G��ԂȂ玀�S
		// �����łȂ��ꍇ�̓v���C���[�Ƀ_���[�W��^����
		if (playerScript->IsUnrivaled()) {
			game->DestroyObject(gameObject);
		}
		else {
			playerScript->Damage(mDamage);
		}
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Enemy::OnCollideEnter(Actor* target){
	if (target->Name() == "Floor") {
		mIsFloorHit = true;
	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Enemy::OnCollideExit(Actor* target){
	if (target->Name() == "Floor") {
		mIsFloorHit = false;
	}
}
// �G�̐F�̐ݒ�ł�
void Enemy::PlayerColorChange() {
	// �F�̐ݒ�
	auto color = XMFLOAT4(1, 1, 1, 1);
	// ��苗�������ƐF�̒l��ς���
	if (mEnemyState == EnemyState::PlayerChase) {
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
// ���G�ړ������܂��i�f�t�H���g�ݒ�j
void Enemy::SearchMove() {
	// �f�t�H���g�̈ړ�
	auto position = gameObject->mTransform->Position();
	auto forward = gameObject->mTransform->Forward();
	gameObject->mTransform->Position(position + forward);
}
// ���G�֐��ł�
void Enemy::PlayerSearchMode(const XMVECTOR objScale) {
	// �e�̎擾
	mParentObj = gameObject->mTransform->GetParent();

	// ���G�I�u�W�F�̍쐬
	if (mAddSearchObjCount < 1) {
		mPlayerSearchObj = game->CreateActor("Assets/PlayerSearchRange");
		game->AddObject(mPlayerSearchObj);
		//game->Debug()->Log("�e����");

		// ���G�I�u�W�F�̃X�N���v�g�擾
		searchScript = mPlayerSearchObj->GetScript<PlayerSearch>();
		// �傫���̕ύX
		auto setScale = XMVectorSet(
			objScale.x * mScalarX, objScale.y * mScalarY, objScale.z * mScalarZ, 0.0f);
		mPlayerSearchObj->mTransform->Scale(setScale);
		searchScript->SetScalarZ(mScalarZ);

		// ���G�͈̓I�u�W�F�̈ʒu�ݒ�
		auto childrenScale = mPlayerSearchObj->mTransform->Scale();
		// �ʒu�̎w��
		auto setPosition = XMVectorSet(0.0f, 0.0f, (objScale.z + childrenScale.z) / 2.0f, 0.0f);
		mSearchObjPosition = setPosition;

		// �ʒu�̕ύX
		mPlayerSearchObj->mTransform->Position(-setPosition);
		game->Debug()->Log(std::to_string(mPlayerSearchObj->mTransform->Position().z));

		// �G�̎q���ɒǉ�����
		mPlayerSearchObj->mTransform->SetParent(mParentObj);

		mAddSearchObjCount = 1;
	}

	// ���G�I�u�W�F�̈ʒu�X�V
	mPlayerSearchObj->mTransform->Position(-mSearchObjPosition);

	if (searchScript->IsPlayerSearch()|| mIsAttckMode){
		if (!mIsDistanceAct) {
			mEnemyState = EnemyState::PlayerChase;
		}
		else {
			auto searchObjScale = mPlayerSearchObj->mTransform->Scale().z;
			/*int PlayerDistanceNumber = 1 + (searchScript->PlayerDistance() / (searchObjScale / 3.0f));
			if (PlayerDistanceNumber == 4) {
				PlayerDistanceNumber = 3;
			}*/
			// �z��ɂ���if���𖳂���
			if (searchScript->PlayerDistance() <= searchObjScale / 3.0f) {
				mEnemyState = EnemyState::PlayerShortDistance;
			}
			else if (searchScript->PlayerDistance() <= searchObjScale / 2.0f) {
				mEnemyState = EnemyState::PlayerCenterDistance;
			}
			else if (searchScript->PlayerDistance() > searchObjScale / 2.0f) {
				mEnemyState = EnemyState::PlayerLongDistance;
			}
		}
	}
	else {
		mEnemyState = EnemyState::PlayerSearch;
	}
}

// �v���C���[�̕����ɉ�]���܂�(������)
void  Enemy::PlayerChaseMode() {
	// �v���C���[�̑{��
	// �v���C���[�̕����������i�܂��j
	auto playerObj = game->FindActor("Board");
	auto playerPosition = playerObj->mTransform->Position();
	// �e�̃X�e�[�^�X�̎擾
	auto parentPosition = mParentObj->mTransform->Position();
	
	auto v = parentPosition - playerPosition;
	auto angle = atan2(v.x, v.z);

	mParentObj->mTransform->Quaternion(XMQuaternionRotationAxis(mParentObj->mTransform->Up(), angle));
}
// �v���C���[��ǐՂ��܂��i�f�t�H���g�ݒ�j
void Enemy::PlayerChase() {
	// �ړ�
	auto position = gameObject->mTransform->Position();
	auto forward = gameObject->mTransform->Forward();
	gameObject->mTransform->Position(position + forward);
}

// �v���C���[�Ƃ̋������Z���Ƃ��̍s���ł��i�f�t�H���g�ݒ�j
void Enemy::ShortDistanceAttack() {
	// �ړ�
	auto position = gameObject->mTransform->Position();
	auto forward = gameObject->mTransform->Forward();
	gameObject->mTransform->Position(position + forward);
}

// �v���C���[�Ƃ̋��������Ԃ̍s���ł��i�f�t�H���g�ݒ�j
void Enemy::CenterDistanceAttack() {
	// �ړ�
	auto position = gameObject->mTransform->Position();
	auto forward = gameObject->mTransform->Forward();
	gameObject->mTransform->Position(position + forward);
}

// �v���C���[�Ƃ̋����������Ƃ��̍s���ł��i�f�t�H���g�ݒ�j
void Enemy::LongDistanceAttack() {
	// �ړ�
	auto position = gameObject->mTransform->Position();
	auto forward = gameObject->mTransform->Forward();
	gameObject->mTransform->Position(position + forward);
}

// �G�̖ڂ̑O�ɃI�u�W�F�𐶐����܂�
void Enemy::SetForwardObj(Actor* setObj) {
	auto parentPosition = mParentObj->mTransform->Position();
	auto parentRotate = mParentObj->mTransform->Rotate();
	auto parentScale = gameObject->mTransform->Scale();
	/*auto objPosition = gameObject->mTransform->Position();
	auto objScale = gameObject->mTransform->Scale();*/
	auto setObjScale = setObj->mTransform->Scale();

	// �ʒu�̎w��
	auto setPosition = XMVectorSet(
		((parentScale.z / 2.0f) + (setObjScale.z / 2.0f)) * sinf(parentRotate.y),
		0.0f,
		((parentScale.z / 2.0f) + (setObjScale.z / 2.0f)) * cosf(parentRotate.y), 0.0f);
	// �ʒu�̕ύX
	setObj->mTransform->Position(parentPosition + -setPosition);
	// �������̃I�u�W�F�̉�]�p�ɕύX
	setObj->mTransform->Rotate(parentRotate);
}

// �_���[�W�̐ݒ�ł�
void Enemy::setDamage(int damage) {
	mDamage = damage;
}

// ��苗���܂ŗ������玀�S���܂�
void Enemy::FallDead(Actor* thisObj) {
	// ���̈ʒu�܂ŗ������玀�S
	if (thisObj->mTransform->Position().y <= mDeadHeigth) {
		game->DestroyObject(mPlayerSearchObj);
		game->DestroyObject(gameObject);
		game->DestroyObject(thisObj);
	}
}

// �v���C���[�Ƃ̋����̌v�Z�����܂� (Z���W)
float Enemy::PlayerDistance() {
	// �v���C���[�̌���
	auto object = game->FindActor("Board");
	//if (!object) return 0.0f;
	// �v���C���[�Ƃ̋����̌v�Z
	auto targetRange = XMVector3Length(
		object->mTransform->Position() - gameObject->mTransform->Position());
	return targetRange.z;
}
// �G�̍s���֐�
void Enemy::Move() {
	PlayerSearchMode(gameObject->mTransform->Scale());

	if (mEnemyState == EnemyState::PlayerSearch) {
		SearchMove();
	}

	if (!mIsDistanceAct) {
		if (mEnemyState == EnemyState::PlayerChase) {
			PlayerChase();
			//PlayerChaseMode();
		}
	}
	else {

		if (mEnemyState == EnemyState::PlayerShortDistance) {
			ShortDistanceAttack();
		}
		else if (mEnemyState == EnemyState::PlayerCenterDistance) {
			CenterDistanceAttack();
		}
		else if (mEnemyState == EnemyState::PlayerLongDistance) {
			LongDistanceAttack();
		}
	}

	Enemy::PlayerColorChange();
	Enemy::FallDead(mParentObj);
}