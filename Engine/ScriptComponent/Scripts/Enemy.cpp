#include "Enemy.h"
#include "PlayerSearch.h"
#include "SailBoard.h"
#include "ParentObj.h"
#include "EnemyCG.h"
#include "MoveSmoke.h"

//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"
#include "Engine\DebugEngine.h"

Enemy::~Enemy() {
	//gameObject->mTransform->AllChildrenDestroy();
	//game->DestroyObject(mEnemyCGObj);
}

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Initialize(){

	mEnemyState = EnemyState::PlayerSearch;
	mAddSearchObjCount = 0;
	mDamage = 0;
	mResPawnTime = 0;
	mCGCreateCount = 0;
	mAnimationID = 0;
	mInitSetCount = 0;
	mSpeed = 1.0f * 0.01f;
	mSize = gameObject->mTransform->Scale();
	mHalfSizeZ = mSize.z / 2.0f;
	mPositionY = gameObject->mTransform->Position().y;
	mRotateY = 0.0f;
	mScalarX = 1.0f;
	mScalarY = 2.0f;
	mScalarZ = 3.0f;
	mAddPlayerChaseStopDistance = 0.0f;
	mTornadoDistance = 0.0f;
	mBlowAwayY = 3.141593f / 5.0f;
	mBlowAwayPower = 20.0f;
	mBlowAwayInterval = 4.0f;
	mIsMove = true;
	mIsFloorHit = false;
	mIsCloudHit = false;
	mIsImmortalBody = false;
	mIsDistanceAct = false;
	mIsBlowAway = true;
	mIsKnockBackDirection = false;
	mIsAttckMode = false;
	mIsChaseRotate = true;
	mIsTornadoRange = false;
	mIsTornadoBlowAway = false;
	mIsDead = false;
	// �z��ɒǐՍs����enum�N���X������
	mDistanceVector.push_back(EnemyState::PlayerShortDistance);
	mDistanceVector.push_back(EnemyState::PlayerCenterDistance);
	mDistanceVector.push_back(EnemyState::PlayerLongDistance);
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Start(){
	// �e�̎擾	
	mParentObj = gameObject->mTransform->GetParent();
	// �����X�e�[�^�X������
	mInitPosition = mParentObj->mTransform->Position();
	mInitRotate = mParentObj->mTransform->Rotate();
}

//���t���[���Ă΂�܂�
void Enemy::Update(){
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Finish(){
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Enemy::OnCollideBegin(Actor* target){
	if (target->Name() == "Board"){
		auto playerScript = target->GetScript<SailBoard>();
		//// �v���C���[�����G��ԂȂ玀�S
		//// �����łȂ��ꍇ�̓v���C���[�Ƀ_���[�W��^����
		//if (playerScript->IsUnrivaled() || playerScript->IsTrick()) {
		//	game->DestroyObject(gameObject);
		//}
		//else {
		//	playerScript->Damage(mDamage);
		//}

		//// �T�E���h�̍Đ�
		//Enemy::EnemyPlaySound("hit");
		// ���S�����u�Ԃɓ����蔻����g���K�[�ɂ���
		auto collider = gameObject->GetComponent<PhysXColliderComponent>();
		collider->SetIsTrigger(true);
		// �v���C���[�̕��ɉ�]
		auto parentPosition = mParentObj->mTransform->Position();
		auto playerPosition = target->mTransform->Position();
		auto v = parentPosition - playerPosition;
		auto angle = atan2(v.x, v.z);
		auto quaternion = XMQuaternionRotationAxis(mParentObj->mTransform->Up(), angle);
		// �_���[�W��^���Ď���������(���ݒ�)
		playerScript->Damage(mDamage);
		mIsDead = true;
		// ���S�A�j���[�V�����̌Ăяo��
		Enemy::SetAnimationID(2);
		Enemy::SetAnimationLoop(false);
	}
	// �����ɓ��������玀�S����
	if (target->Name() == "Tornado") {
		mIsDead = true;
		mIsBlowAway = true;
		mBlowAwayPower = 100.0f;
		// �R���C�_�[�̃g���K�[���I���ɂ���
		auto collider = gameObject->GetComponent<PhysXColliderComponent>();
		collider->SetIsTrigger(true);
		//mBlowAwayInterval = 1.0f;
	}

	if (target->Name() == "Floor") {
		mInitSetCount = 1;
		mIsFloorHit = true;
	}
	// �_�Ɠ��������������t���O���ԐړI��true�ɂ���
	if (target->Name() == "Cloud_s" ||
		target->Name() == "Cloud_s1" ||
		target->Name() == "Cloud_s2") {
		mIsCloudHit = true;
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Enemy::OnCollideEnter(Actor* target){
	if (target->Name() == "Floor") mIsFloorHit = true;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Enemy::OnCollideExit(Actor* target){
	if (target->Name() == "Floor") mIsFloorHit = false;
}
// �G�̐F�̐ݒ�ł�(�f�o�b�O�p)
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
	if (mate) mate->SetAlbedoColor(color);
	//auto color = mate->GetMaterial();
	//auto albedoColor = mate->mAlbedo;
	//auto color = XMVectorSet(albedoColor.x, albedoColor.y, .)
	//if(mate) mate->SetAlbedoColor()
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
	// ���G�͈̓I�u�W�F�̐�����ɒǐՒ��~�����̐ݒ���s��
	if (mAddSearchObjCount == 1) {
		// �ǐՒ��~�̋����̐ݒ�
		mSearchScript->AddChaseStopDistance(mAddPlayerChaseStopDistance);
		mAddSearchObjCount = 2;
	}
	// ���G�I�u�W�F�̍쐬
	if (mAddSearchObjCount < 1) {
		mPlayerSearchObj = game->CreateActor("Assets/Enemy/PlayerSearchRange");
		game->AddObject(mPlayerSearchObj);
		mPlayerSearchObj->mTransform->SetParent(mParentObj);
		// ���G�I�u�W�F�̃X�N���v�g�擾
		mSearchScript = mPlayerSearchObj->GetScript<PlayerSearch>();
		// �傫���̕ύX
		auto setScale = XMVectorSet(
			objScale.x * mScalarX, objScale.y * mScalarY, objScale.z * mScalarZ, 0.0f);
		mPlayerSearchObj->mTransform->Scale(setScale);
		mSearchScript->SetScalarZ(mScalarZ);
		// ���G�͈̓I�u�W�F�̈ʒu�ݒ�
		auto childrenScale = mPlayerSearchObj->mTransform->Scale();
		// �ʒu�̎w��
		auto setPosition = XMVectorSet(0.0f, 0.0f, (objScale.z + childrenScale.z) / 2.0f, 0.0f);
		mSearchObjPosition = setPosition;
		// �ʒu�̕ύX
		mPlayerSearchObj->mTransform->Position(-setPosition);
		// �G�̎q���ɒǉ�����
		mPlayerSearchObj->mTransform->SetParent(mParentObj);

		// CG�̐���
		std::string baseName = "Assets/Enemy/EnemyCGObj/" + gameObject->Name() + "CG";
		auto createCGObjName = baseName.c_str();
		auto CGObj = game->CreateActor(createCGObjName);
		game->AddObject(CGObj);
		mEnemyCGObj = CGObj;
		//CGObj->mTransform->SetParent(gameObject);
		CGObj->mTransform->SetParent(mParentObj);
		mEnemyCGScript = CGObj->GetScript<EnemyCG>();
		// ��x�����������x�Ɛ������Ȃ�
		mAddSearchObjCount = 1;
	}
	// ���G�I�u�W�F�̈ʒu�X�V
	mPlayerSearchObj->mTransform->Position(-mSearchObjPosition);
	// ���G�͈͓��Ƀv���C���[����������
	if (mSearchScript->IsPlayerSearch()){
		if (!mIsDistanceAct) {
			mEnemyState = EnemyState::PlayerChase;
		}
		else {
			if (!mIsAttckMode) {
				auto searchObjScale = mPlayerSearchObj->mTransform->Scale().z;
				// �ǐՍs���̐������v�Z
				int playerDistanceNumber = mSearchScript->GetPlayerDistance() / (searchObjScale / 3.0f);
				if (playerDistanceNumber >= 3) {
					playerDistanceNumber = 2;
				}
				// �G�̒ǐՍs���̌���
				mEnemyState = mDistanceVector[playerDistanceNumber];
			}
		}
	}
	else {
		if (!mIsAttckMode) {
			mEnemyState = EnemyState::PlayerSearch;
		}
	}
}

// �v���C���[�̕����ɉ�]���܂�
void  Enemy::PlayerChaseMode(const float startPoint, const float endPoint) {
	// �v���C���[�̑{��
	// �v���C���[�̕���������
	auto playerObj = game->FindActor("Board");
	//if(playerObj != nullptr)
	auto playerPosition = playerObj->mTransform->Position();
	// �e�̃X�e�[�^�X�̎擾
	auto parentPosition = mParentObj->mTransform->Position();
	// �v���C���[�Ƃ̃x�N�g���̌������v�Z
	auto v = parentPosition - playerPosition;
	auto angle = atan2(v.x, v.z);
	auto quaternion = XMQuaternionRotationAxis(mParentObj->mTransform->Up(), angle);
	// �񎟌��̋��������߂�
	auto v2PlayerPosition = XMVectorSet(playerPosition.x, 0.0f, playerPosition.z, 0.0f);
	auto v2ParentPosition = XMVectorSet(parentPosition.x, 0.0f, parentPosition.z, 0.0f);
	auto playerDistance = XMVector3Length(v2PlayerPosition - v2ParentPosition);
	//�R���C�_�[�̑傫�����擾
	auto collider = gameObject->GetComponent<PhysXColliderComponent>();
	auto colliderScale = collider->GetScale();
	// �v���C���[�Ƃ̋������v���C���[�̈��̑傫���ȉ��Ȃ��]���Ȃ�
	if (playerDistance.z >= colliderScale.z / 2.0f + startPoint)mIsChaseRotate = true;
	else if (playerDistance.z < colliderScale.z / 2.0f + endPoint)mIsChaseRotate = false;
	// mIsChaseRotate �� true �Ȃ�v���C���[�̕��ɉ�]
	// ����ɂ��A�G���v���C���[��ǐՂ���Ƃ��Ɋp�΂������������Ȃ��Ȃ�
	if (mIsChaseRotate)mParentObj->mTransform->Quaternion(quaternion);
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

// �������瓦����Ƃ��̍s���ł�
void Enemy::TornadoEscapeMove(Actor* tornadoObj) {
	mIsMove = true;
	auto tornadoPosition = tornadoObj->mTransform->Position();
	// �e�̃X�e�[�^�X�̎擾
	auto parentPosition = mParentObj->mTransform->Position();
	// �����Ƃ̉�]�������v�Z(������悤�ɂ���)
	auto v = parentPosition - tornadoPosition;
	auto angle = atan2(v.x, v.z);
	// angle��90�����Z����
	auto escapeAngle = angle - 3.141593f / 2.0f;
	// escapeAngle �� 0.000000f �ɂȂ��Ă����Ԃł𑫂��ƃG���[�Ȏ��ƂȂ�
	// ���O�\���ł� escapeAngle �� 0.000000f �ƂȂ��Ă��邪�A0.000000f�ƔF�����Ă���Ȃ�
	// string�^���ƔF�����Ă����
	// if (std::to_string(escapeAngle) == "0.000000") game->Debug()->Log("��]���G���[");
	if (std::to_string(escapeAngle) == "0.000000") escapeAngle = 0.0f;
	if (escapeAngle < 0) escapeAngle += 3.141593f * 2.0f;
	// �������瓦����悤�Ɉړ�����
	mParentObj->mTransform->Rotate(mParentObj->mTransform->Up() * (escapeAngle));
	mParentObj->mTransform->Position(
		parentPosition +
		((mParentObj->mTransform->Forward() +
			(-mParentObj->mTransform->Left() / mTornadoInterval))
			* (mTornadoPower * 0.01f)) * Enemy::GetEnemyDeltaTime(60.0f));
	auto distance = XMVector3Length(tornadoPosition - parentPosition);
	if (distance.x <= 8.0f) mTornadoInterval = 2.0f;

	Enemy::SetAnimationID(0);
}

// �v���C���[�ɓ���������m�b�N�o�b�N���܂�
void Enemy::KnockBack() {
	auto parentPosition = mParentObj->mTransform->Position();
	// �m�b�N�o�b�N�̕��������߂�
	if (!mIsKnockBackDirection) {
		// �v���C���[�̕��ɉ�]
		auto player = game->FindActor("Board");
		auto playerPosition = player->mTransform->Position();
		auto v2ParentPosition = XMVectorSet(parentPosition.x, 0.0f, parentPosition.z, 0.0f);
		auto v2PlayerPosition = XMVectorSet(playerPosition.x, 0.0f, playerPosition.z, 0.0f);
		auto v2 = v2ParentPosition - v2PlayerPosition;
		mKnockBackDIrection = XMVector3Normalize(v2);
		mIsKnockBackDirection = true;
	}
	// ���ɋ������ė�����
	mParentObj->mTransform->Position(
		parentPosition + ((mKnockBackDIrection * 20.0f * 0.01f) -
		mParentObj->mTransform->Up() * 5.0f * 0.01f) * Enemy::GetEnemyDeltaTime(60.0f));
	Enemy::Dead();
}

// �G�̖ڂ̑O�ɃI�u�W�F�𐶐����܂�(�e������ꍇ)
void Enemy::SetParentForwardObj(Actor* setObj) {
	auto parentPosition = mParentObj->mTransform->Position();
	auto parentRotate = mParentObj->mTransform->Rotate();
	auto collider = gameObject->GetComponent<PhysXColliderComponent>();
	auto colliderScale = collider->GetScale();
	auto setObjScale = setObj->mTransform->Scale();
	// �ʒu�̎w��
	auto setPosition = XMVectorSet(
		((colliderScale.z / 2.0f) + (setObjScale.z / 2.0f)) * sinf(parentRotate.y),
		0.0f,
		((colliderScale.z / 2.0f) + (setObjScale.z / 2.0f)) * cosf(parentRotate.y), 0.0f);
	// �ʒu�̕ύX
	setObj->mTransform->Position(parentPosition + -setPosition);
	// �������̃I�u�W�F�̉�]�p�ɕύX
	setObj->mTransform->Rotate(parentRotate);
}

// �_���[�W�̐ݒ�ł�
void Enemy::SetDamage(int damage) {
	mDamage = damage;
}

// ���X�|�[���^�C���ł�
void Enemy::SetResPawnTime(int time) {
	mResPawnTime = time;
	mInitResPawnTime = time;
}

// ���G�͈͂̃T�C�Y��ύX���܂�
void Enemy::SetSearchRangeScale(const float scaleX, const float scaleY, const float scaleZ) {
	mScalarX = scaleX;
	mScalarY = scaleY;
	mScalarZ = scaleZ;
}

// �ړ����̃X���b�N�I�u�W�F�𐶐����܂�
void Enemy::EnemyMoveSmoke() {
	// ���ɐG��Ă����琶��
	// ���ɐG��Ă��Ȃ�������폜����
	if (mIsFloorHit) {
		if (mIsMove) {
			if (mRightSmokeObj == nullptr) {
				// �q�̐��� �����̖��O����q��I�����������܂�
				std::string baseName = "Assets/Enemy/" + gameObject->Name() + "MoveSmoke";
				auto createObjName = baseName.c_str();
				// �E�̃X���b�N����
				mRightSmokeObj = game->CreateActor(createObjName);
				game->AddObject(mRightSmokeObj);
				mRightSmokeObj->mTransform->SetParent(gameObject);
				// ���̃X���b�N����
				mLeftSmokeObj = game->CreateActor(createObjName);
				game->AddObject(mLeftSmokeObj);
				mLeftSmokeObj->mTransform->SetParent(gameObject);
			}
			// �R���C�_�[�Œ�������
			auto collider = gameObject->GetComponent<PhysXColliderComponent>();
			auto colliderScale = collider->GetScale();
			auto enemyCGPosition = mEnemyCGObj->mTransform->Position();
			auto radius = 3.141593f / 180.0f;
			// �E�̃X���b�N�̃X�e�[�^�X�ݒ�
			auto rightX = -colliderScale.x / 2.0f - enemyCGPosition.x;
			auto rightZ = -colliderScale.z / 2.0f + enemyCGPosition.z;
			auto rightPosition = XMVectorSet(rightX, 0.0f, rightZ, 0.0f);
			mRightSmokeObj->mTransform->Position(rightPosition);
			// ���̃X���b�N�̃X�e�[�^�X�ݒ�
			auto leftX = colliderScale.x / 2.0f + enemyCGPosition.x;
			auto leftPosition = XMVectorSet(leftX, 0.0f, rightZ, 0.0f);
			mLeftSmokeObj->mTransform->Position(leftPosition);
			mRightSmokeScript = mRightSmokeObj->GetScript<MoveSmoke>();
			mLeftSmokeScript = mLeftSmokeObj->GetScript<MoveSmoke>();
		}
		else {
			if (mRightSmokeObj == nullptr)return;
			mRightSmokeScript->SetSpeed(0.0f);
			mLeftSmokeScript->SetSpeed(0.0f);
		}
	}
	else {
		if (mRightSmokeObj == nullptr)return;
		mRightSmokeScript->SetSpeed(0.0f);
		mLeftSmokeScript->SetSpeed(0.0f);
	}
}

// ��苗���܂ŗ������烊�X�|�[�����܂�
void Enemy::ResPawnLine() {
	// ���̈ʒu�܂ŗ������烊�X�|�[���^�C�������Z
	if (mParentObj->mTransform->Position().y <= mResPawnHeigth) {
		// ���X�|�[���^�C�����O�Ȃ烊�X�|�[������
		if (mResPawnTime < 0) {
			Enemy::InitStatus();
			mResPawnTime = mInitResPawnTime;
			mInitSetCount = 0;
		}
		mResPawnTime--;
	}
}

// ������T���܂�
void Enemy::SearchTornado() {
	// �����̐e����
	auto tornadoParent = game->FindActor("Tornados");
	if (mTornadoMinDistance == 0.0f) mTornadoMinDistance = mTornadoDistance;
	if (tornadoParent != nullptr) {
		auto tornadoChildren = tornadoParent->mTransform->Children();
		// �q��������Ȃ痳���Ƃ̋��������߂�
		auto size = tornadoChildren.size();
		if (size >= 2) {
			// �q�̒�����ŒZ�����̗�����T��
			for (std::list<Actor*>::iterator i = tornadoChildren.begin(); i != tornadoChildren.end(); i++) {
				auto tathumaki = *i;
				// �����Ƃ̋����̌v�Z
				auto tornadoDistance = XMVector3Length(
					tathumaki->mTransform->Position() -
					mParentObj->mTransform->Position());
				// �ŒZ�����Ȃ�X�V����
				if (tornadoDistance.x <= mTornadoMinDistance) {
					// �ŒZ����������
					mTornadoMinDistance = tornadoDistance.x;
					// �����I�u�W�F������
					mTornadoObj = tathumaki;
				}
			}
			// �����Ƃ̋�������苗�����Ȃ痳���̕����Ɉړ�����
			if (mTornadoMinDistance <= mTornadoDistance) {
				mIsTornadoRange = true;
			}
		}
		else if (size == 1) {
			auto tathumaki = game->FindActor("Tornado");
			if (tathumaki != nullptr) {
				auto distance = XMVector3Length(
					tathumaki->mTransform->Position() - mParentObj->mTransform->Position());
				// �����Ƃ̋�������苗�����Ȃ痳���̕����Ɉړ�����
				if (distance.x <= mTornadoDistance) {
					mIsTornadoRange = true;
					mTornadoObj = tathumaki;
				}
			}
			else {
				// �����Ɠ������Ă��Ȃ��āA�r���ŗ������Ȃ��Ȃ�����ʏ�s���ɖ߂�
				mIsTornadoRange = false;
				mTornadoObj = nullptr;
				mTornadoMinDistance = mTornadoDistance;
			}
		}
		else if (size == 0) {
			// �������Ȃ��Ȃ�ʏ�s��������
			mIsTornadoRange = false;
			mTornadoObj = nullptr;
			mTornadoMinDistance = mTornadoDistance;
		}
	}
}

// �G�̎��S�s���ł�
void Enemy::DeadMove() {
	auto position = mParentObj->mTransform->Position();
	// ������ԏꍇ
	if (mIsBlowAway) {
		// �����̐������
		if (mIsTornadoRange) {
			// �㏸
			// �������Ȃ��Ȃ��Ă����炻�̏�Ő�����΂�
			if (mTornadoObj == nullptr) {
				mIsTornadoBlowAway = true;
			}
			// �����̃R���C�_�[�̎擾
			auto tornadoCollider = mTornadoObj->GetComponent<PhysXColliderComponent>();
			auto tornadoColliderScale = tornadoCollider->GetScale();
			// �����ɉ����ĉ�]
			auto tornadoPosition = mTornadoObj->mTransform->Position();
			// �e�̃X�e�[�^�X�̎擾
			auto parentPosition = mParentObj->mTransform->Position();
			// �����̍����ȏ�ɂȂ�����ʕ����ɐ�����΂�
			if (parentPosition.y >= tornadoColliderScale.y) {
				mIsTornadoBlowAway = true;
			}
			// �����𖞂�������ʕ����ɐ�����΂�
			if (!mIsTornadoBlowAway) {
				// ���[���h��Y���W�ŏ㏸����
				auto vectorY = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
				auto up = vectorY * cosf(mParentObj->mTransform->Rotate().x) * 0.01f * mBlowAwayPower;
				// �����Ƃ̉�]�������v�Z(����������ƈꏏ�ɂ���)
				auto v = parentPosition - tornadoPosition;
				auto angle = atan2(v.x, v.z);
				auto escapeAngle = angle - 3.141593f / 2.0f;
				// ��]�␳
				if (std::to_string(escapeAngle) == "0.000000") escapeAngle = 0.0f;
				if (escapeAngle < 0) escapeAngle += 3.141593f * 2.0f;
				// �����̂悤�ɉ�]���Ȃ��琁�����
				mParentObj->mTransform->Rotate(mParentObj->mTransform->Up() * (escapeAngle));
				mParentObj->mTransform->Position(
					parentPosition + 
					((up * mTornadoUpPower * 0.01f) +
					(mParentObj->mTransform->Forward() -
						(mParentObj->mTransform->Left() / (mTornadoRotateScale))) * 
					(mTornadoRotatePower * 0.01f)) * Enemy::GetEnemyDeltaTime(60.0f));

				mEnemyCGObj->mTransform->Rotate(mParentObj->mTransform->Left() * 1.0f);
				mTornadoRotateScale += mAddTornadoRotateScale;
			}
			else {
				mParentObj->mTransform->Position(
					parentPosition + 
					(mParentObj->mTransform->Forward() +
					(mParentObj->mTransform->Up() * cos(mBlowAwayY))) * Enemy::GetEnemyDeltaTime(60.0f));
				if (mBlowAwayY < 3.141593f) {
					mBlowAwayY += 3.141593f / 180.0f;
				}

				Enemy::Dead();
			}

			Enemy::SetAnimationID(2);
			//Enemy::SetAnimationID(false);
			if (Enemy::GetAnimationTime() > 10.0f) {
				Enemy::SetAnimationTime(10.0f);
			}
		}
		else {
			// �����胂�[�V����
			// 180�x�܂ŉ�]
			if (mParentObj->mTransform->Rotate().x < 3.141593f) {
				// ��]
				auto rotate = mParentObj->mTransform->Rotate();
				rotate.x += 3.141593f / (180.0f / mBlowAwayInterval);
				mParentObj->mTransform->Rotate(rotate);
			}
			else {
				// ��]�������ď����𖞂������玀�S
				Enemy::Dead();
			}
			// �㏸
			auto up = mParentObj->mTransform->Up() *
				cosf(gameObject->mTransform->Rotate().x) * 0.01f * mBlowAwayPower;
			mParentObj->mTransform->Position(position + up * Enemy::GetEnemyDeltaTime(60.0f));
		}
	}
	else {
		// �m�b�N�o�b�N���[�V����
		Enemy::KnockBack();
	}
	// �ēx�G��Ă��_���[�W����
	Enemy::SetDamage(0);
}

// ���S�������s���܂�
void Enemy::Dead() {
	auto parentPosition = mParentObj->mTransform->Position();
	// �_�ɓ������������
	if (mIsCloudHit) {
		game->DestroyObject(mPlayerSearchObj);
		game->DestroyObject(mEnemyCGObj);
		game->DestroyObject(gameObject);
		game->DestroyObject(gameObject->mTransform->GetParent());
	}
	else {
		// ���X�|�[���ʒu�܂ŗ����������
		if (parentPosition.y <= mResPawnHeigth) {
			game->DestroyObject(mPlayerSearchObj);
			game->DestroyObject(mEnemyCGObj);
			game->DestroyObject(gameObject);
			game->DestroyObject(gameObject->mTransform->GetParent());
		}
	}
}

// �G�̍s���֐�
void Enemy::Move() {
	// ������T��
	Enemy::SearchTornado();
	// ����ł��Ȃ�������e�G�̍s�����s��
	if (!mIsDead) {
		auto collider = gameObject->GetComponent<PhysXColliderComponent>();
		auto colliderScale = collider->GetScale();
		PlayerSearchMode(colliderScale);
		mPlayerSearchObj->mTransform->SetParent(mParentObj);
		// �����͈͓̔��ɓ����Ă��Ȃ�������ʏ�s�����s��
		if (!mIsTornadoRange) {
			if (mEnemyState == EnemyState::PlayerSearch) {
				// �v���C���[��{������s��
				SearchMove();
			}
			else {
				if (!mIsDistanceAct) {
					// �v���C���[���������Ƃ��̍s��
					if (mEnemyState == EnemyState::PlayerChase) {
						PlayerChase();
					}
				}
				else {
					// �����ł̍s��
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
			}
		}
		else {
			// �����Ɋ������܂ꂽ�Ƃ��̍s��
			Enemy::TornadoEscapeMove(mTornadoObj);
		}
		// ���X�|�[�����邩�̔���
		Enemy::ResPawnLine();
	}
	else {
		// ���S�s���̌Ăяo��
		Enemy::DeadMove();
	}
	// �X�e�[�^�X�̃��Z�b�g
	Enemy::ResetStatus();
	// �X���[�N�̌Ăяo��
	Enemy::EnemyMoveSmoke();
}

// �e�̃X�e�[�^�X�̏�����
void Enemy::InitStatus() {
	mParentObj->mTransform->Position(mInitPosition);
	mParentObj->mTransform->Rotate(mInitRotate);
}

// �G�̃X�e�[�^�X�̃��Z�b�g
void Enemy::ResetStatus() {
	auto ZeroStatus = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	// �O�̒l�ɕς���
	gameObject->mTransform->Position(ZeroStatus);
	gameObject->mTransform->Rotate(ZeroStatus);
}

// �v���C���[�̒ǐՂ𒆎~���鋗���̉��Z�ł�
void Enemy::AddPlayerChaseStopDistance(float distance) {
	mAddPlayerChaseStopDistance = distance;
}

// �T�E���h���Đ����܂�
void Enemy::EnemyPlaySound(const std::string soundName) {
	std::string playSoundName = "Assets/" + soundName;
	//// �T�E���h��炷
	auto sound = gameObject->GetComponent<SoundComponent>();
	if (sound) return;
	/*sound->LoadFile("playSoundName");
	sound->Play();*/
}

// �����̃X�e�[�^�X�����܂�
void Enemy::SetTornadoStatus(
	const float power, const float rotate, const float addRotate,
	const float rotatePower, const float upPower, const float interval,
	const float distance) {
	mTornadoPower = power;
	mTornadoRotateScale = rotate;
	mAddTornadoRotateScale = addRotate;
	mTornadoRotatePower = rotatePower;
	mTornadoUpPower = upPower;
	mTornadoInterval = interval;
	mTornadoDistance = distance;
}

// �v���C���[�Ǝw�肳�ꂽ�I�u�W�F�̈ʒu�Ƃ̋������v�Z���ĕԂ��܂�
float Enemy::GetPlayerDistance(Actor* playerObj, Actor* otherObj) {
	auto playerPosition = playerObj->mTransform->Position();
	auto otherPosition = otherObj->mTransform->Position();
	// �v���C���[�Ƃ̋������v�Z
	auto distance = XMVector3Length(playerPosition - otherPosition);
	return distance.z;
}

// �q��������A�j���[�V������ID��ς��܂�
void Enemy::SetAnimationID(int id) {
	mEnemyCGScript->SetAnimationID(id);
}

// �q��������A�j���[�V�����̃^�C����ύX���܂�
void Enemy::SetAnimationTime(float time) {
	mEnemyCGScript->SetAnimationTime(time);
}

// �A�j���[�V�����̃^�C���X�P�[����ύX���܂�
void Enemy::SetAnimationTimeScale(float timeScale) {
	mEnemyCGScript->SetTimeScale(timeScale);
}

// �q��������A�j���[�V�����̃��[�v��ύX���܂�
void Enemy::SetAnimationLoop(bool isLoop) {
	mEnemyCGScript->SetLoop(isLoop);
}

// ���݂̃A�j���[�V�����̃^�C�����擾���܂�
float Enemy::GetAnimationTime() {
	return mEnemyCGScript->GetAnimationTime();
}

// �f���^�^�C�����擾���܂�
float Enemy::GetEnemyDeltaTime(float framerate) {
	auto deltaTime = game->DeltaTime()->GetDeltaTime();
	return deltaTime * framerate;
}