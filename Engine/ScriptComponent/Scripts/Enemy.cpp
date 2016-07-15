#include "Enemy.h"
#include "EnemyManager.h"
#include "PlayerSearch.h"
#include "SailBoard.h"
#include "ParentObj.h"
#include "EnemyCG.h"
#include "CreateEnemyObj.h"
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
	mEnemyDeadState = EnemyDeadState::BlowAwayDead;
	mAddSearchObjCount = 0;
	mDamage = 0;
	mResPawnTime = 0;
	mCGCreateCount = 0;
	mAnimationID = 0;
	mInitSetCount = 0;
	mTornadosCount = 1;
	mSpeed = 1.0f * 0.01f;
	mSize = gameObject->mTransform->Scale();
	mHalfSizeZ = mSize.z / 2.0f;
	mPositionY = gameObject->mTransform->Position().y;
	mRotateY = 0.0f;
	mScalarX = 1.0f;
	mScalarY = 2.0f;
	mScalarZ = 3.0f;
	mAddPlayerChaseStopDistance = 0.0f;
	mTornadoDistance = 50.0f;
	mBlowAwayY = 3.141593f / 5.0f;
	mBlowAwayPower = 20.0f;
	mBlowAwayInterval = 4.0f;
	mIsMove = true;
	mIsFloorHit = false;
	mIsCloudHit = false;
	mIsDamage = false;
	mIsImmortalBody = false;
	mIsDistanceAct = false;
	mIsKnockBackDirection = false;
	mIsAttckMode = false;
	mIsChaseRotate = true;
	mIsTornadoBlowAway = false;
	mIsPlayerHeal = false;
	mIsCreateChase = false;
	// �z��ɒǐՍs����enum�N���X������
	mDistanceVector.push_back(EnemyState::PlayerShortDistance);
	mDistanceVector.push_back(EnemyState::PlayerCenterDistance);
	mDistanceVector.push_back(EnemyState::PlayerLongDistance);
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Start(){
	// �e�̎擾	
	mParentObj = gameObject->mTransform->GetParent();
	//// �����X�e�[�^�X������
	//mInitPosition = mParentObj->mTransform->Position();
	//mInitRotate = mParentObj->mTransform->Rotate();
	// �G�l�~�[�}�l�[�W���[�̍쐬
	mEnemyManagerObj = game->CreateActor("Assets/Enemy/EnemyManager");
	game->AddObject(mEnemyManagerObj);
	mEnemyManagerObj->mTransform->SetParent(gameObject);
	mEnemyManagerScript = mEnemyManagerObj->GetScript<EnemyManager>();
	// �����������ł��鐶���I�u�W�F�N�g�̑{��
	//mCreateEnemyObj = game->FindActor("CreateEnemyObj");
	mCreateEnemyObj = mParentObj->mTransform->GetParent();
	if (mCreateEnemyObj != nullptr) {
		if (mCreateEnemyObj->Name() == "CreateEnemyObj") {
			auto createEnemyScript = 
				mCreateEnemyObj->GetScript<CreateEnemyObj>();
			mIsCreateChase = createEnemyScript->GetIsPlayerChaseMode();
		}
	}
}

//���t���[���Ă΂�܂�
void Enemy::Update(){
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Finish(){
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Enemy::OnCollideBegin(Actor* target){
	if (mEnemyState != EnemyState::DeadMove) {
		if (target->Name() == "Board") {
			auto playerScript = target->GetScript<SailBoard>();
			// �v���C���[�����G��ԂȂ玀�S
			// �����łȂ��ꍇ�̓v���C���[�Ƀ_���[�W��^����
			if (playerScript->IsUnrivaled()) {
				playerScript->Damage(-mDamage);
				// ���S�����u�Ԃɓ����蔻����g���K�[�ɂ���
				auto collider = gameObject->GetComponent<PhysXColliderComponent>();
				collider->SetIsTrigger(true);
				// �v���C���[�̕��ɉ�]
				auto parentPosition = mParentObj->mTransform->Position();
				auto playerPosition = target->mTransform->Position();
				auto v = parentPosition - playerPosition;
				auto angle = atan2(v.x, v.z);
				auto quaternion = XMQuaternionRotationAxis(mParentObj->mTransform->Up(), angle);
				mEnemyState = EnemyState::DeadMove;
				// ���S�A�j���[�V�����̌Ăяo��
				Enemy::SetAnimationID(2);
				Enemy::SetAnimationLoop(false);
			}
			else {
				if (!mIsDamage) {
					playerScript->Damage(mDamage);
					mIsDamage = true;
				}
			}
			// �T�E���h�̍Đ�
			mEnemyManagerScript->EnemyPlaySound("hit");
		}
		// �����ɓ��������玀�S����
		if (target->Name() == "Tornado") {
			mEnemyState = EnemyState::DeadMove;
			mEnemyDeadState = EnemyDeadState::TornadoDead;
			mBlowAwayPower = 100.0f;
			mBlowAwayTornadoObj = mTornadoObj;
			mTornadoObj = nullptr;
			// �R���C�_�[�̃g���K�[���I���ɂ���
			auto collider = gameObject->GetComponent<PhysXColliderComponent>();
			collider->SetIsTrigger(true);
		}
	}
	if (target->Name() == "Floor") {
		mInitSetCount = 1;
		mIsFloorHit = true;
		mIsDamage = false;
	}
	if (target->Name() == "Tower" ||
		target->Name() == "bridge" ||
		target->Name() == "Tree") {
		auto KnockBack = mParentObj->mTransform->Forward();
		auto parentPosition = mParentObj->mTransform->Position();
		mParentObj->mTransform->Position(parentPosition + KnockBack);
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
	if (target->Name() == "Floor") {
	
		// ���̊p�x�Ɠ����ɂ���
		auto targetPosition = target->mTransform->Position();
		auto floorRotate = target->mTransform->Rotate();
		auto parentPosition = mParentObj->mTransform->Position();
		auto parentRotate = mParentObj->mTransform->Rotate();
		// �p�x���O�Ȃ�������
		if (abs(floorRotate.x) > abs(floorRotate.z)) {
			if (floorRotate.z == 0) floorRotate.z = floorRotate.x;
			// �p�x�̕ύX
			auto enemyRotate = XMVectorSet(
				floorRotate.x * cosf(parentRotate.y), parentRotate.y,
				floorRotate.z * -sinf(parentRotate.y), 0.0f);
			mParentObj->mTransform->Rotate(enemyRotate);
		}
		else if (abs(floorRotate.x) < abs(floorRotate.z)) {
			if (floorRotate.x == 0) floorRotate.x = floorRotate.z;
			// �p�x�̕ύX
			auto enemyRotate = XMVectorSet(
				floorRotate.x * -sinf(parentRotate.y), parentRotate.y,
				floorRotate.z * cosf(parentRotate.y), 0.0f);
			mParentObj->mTransform->Rotate(enemyRotate);
		}

		mIsFloorHit = true;
		mIsDamage = false;
	}

	// �����ɓ��������玀�S����
	if (target->Name() == "Tornado" && mEnemyState != EnemyState::DeadMove) {
		mEnemyState = EnemyState::DeadMove;
		mEnemyDeadState = EnemyDeadState::TornadoDead;
		mBlowAwayPower = 100.0f;
		mBlowAwayTornadoObj = mTornadoObj;
		// �R���C�_�[�̃g���K�[���I���ɂ���
		auto collider = gameObject->GetComponent<PhysXColliderComponent>();
		collider->SetIsTrigger(true);
	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Enemy::OnCollideExit(Actor* target){
	if (target->Name() == "Floor") mIsFloorHit = false;
}

// ���G�ړ������܂��i�f�t�H���g�ݒ�j
void Enemy::PlayerSearchMove() {
	// �f�t�H���g�̈ړ�
	auto position = gameObject->mTransform->Position();
	auto forward = gameObject->mTransform->Forward();
	gameObject->mTransform->Position(position + forward);
}
// ���G�֐��ł�
void Enemy::PlayerSearchMode(const XMVECTOR objScale) {
	// ���S���Ă����炷���ɕԂ�
	if (mEnemyState == EnemyState::DeadMove) return;
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
		// �������ɒǐՂ��邩
		mSearchScript->SetIsPlayerHit(mIsCreateChase);
		//if (mIsCreateChase) mSearchScript->SetIsPlayerHit(mIsCreateChase);
		// CG�̐���
		std::string baseName = "Assets/Enemy/EnemyCGObj/" + gameObject->Name() + "CG";
		auto createCGObjName = baseName.c_str();
		auto CGObj = game->CreateActor(createCGObjName);
		game->AddObject(CGObj);
		mEnemyCGObj = CGObj;
		mEnemyCGObj->mTransform->SetParent(mParentObj);
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
			return;
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
				return;
			}
		}
	}
	else {
		if (!mIsAttckMode) {
			if (mSearchScript->IsLost()) {
				// �ʏ�s��
				mEnemyState = EnemyState::PlayerSearch;
				// �߂�s��
				if(mCreateEnemyObj->Name() == "CreateEnemyObj")
					mEnemyState = EnemyState::ReturnMove;
				return;
			}
		}
	}
}

// �v���C���[�̕����ɉ�]���܂�
void  Enemy::PlayerChaseMode(const float startPoint, const float endPoint) {
	if (!mIsFloorHit) return;
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
void Enemy::PlayerChaseMove() {
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
	if (tornadoObj == nullptr) return;
	if (tornadoObj->Name() != "Tornado") {
		mTornadoObj = nullptr;
		mTornadoMinDistance = mTornadoDistance;
		return;
	}
	// �X���b�N�𓮂���
	mIsMove = true;
	// �����̈ʒu�̎擾
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
	if (distance.x <= 10.0f) mTornadoInterval = 1.0f;
	auto tornadoCollider = mTornadoObj->GetComponent<PhysXColliderComponent>();
	if (distance.x <= tornadoCollider->GetScale().x) {
		mEnemyState = EnemyState::DeadMove;
		mEnemyDeadState = EnemyDeadState::TornadoDead;
		mBlowAwayPower = 100.0f;
		mBlowAwayTornadoObj = mTornadoObj;
		mTornadoObj = nullptr;
		// �R���C�_�[�̃g���K�[���I���ɂ���
		auto collider = gameObject->GetComponent<PhysXColliderComponent>();
		collider->SetIsTrigger(true);
	}

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
			auto rightPosition = XMVectorSet(rightX, 0.3f, rightZ, 0.0f);
			mRightSmokeObj->mTransform->Position(rightPosition);
			// ���̃X���b�N�̃X�e�[�^�X�ݒ�
			auto leftX = colliderScale.x / 2.0f + enemyCGPosition.x;
			auto leftPosition = XMVectorSet(leftX, 0.3f, rightZ, 0.0f);
			mLeftSmokeObj->mTransform->Position(leftPosition);
			mRightSmokeScript = mRightSmokeObj->GetScript<MoveSmoke>();
			mLeftSmokeScript = mLeftSmokeObj->GetScript<MoveSmoke>();
			mRightSmokeScript->SetSpeed(0.1f);
			mLeftSmokeScript->SetSpeed(0.1f);
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
			mEnemyManagerScript->InitStatus();
			mResPawnTime = mInitResPawnTime;
			mInitSetCount = 0;
		}
		mResPawnTime -= Enemy::GetEnemyDeltaTime(60.0f);
	}
}

// ������T���܂�
void Enemy::SearchTornado() {
	if (mEnemyState == EnemyState::DeadMove) return;
	// �����̐e����
	auto tornadoParent = game->FindActor("Tornados");
	if (tornadoParent != nullptr) {
		auto tornadoChildren = tornadoParent->mTransform->Children();
		// �q��������Ȃ痳���Ƃ̋��������߂�
		auto size = tornadoChildren.size();
		// �������������Ȃ�A���G���[�h�~��nullptr������
		if (mTornadosCount != size) {
			mTornadosCount = size;
			mEnemyState = EnemyState::PlayerSearch;
			mTornadoObj = nullptr;
			mTornadoMinDistance = mTornadoDistance;
			return;
		}
		if (size >= 1) {
			// �q�̒�����ŒZ�����̗�����T��
			for (auto i = tornadoChildren.begin(); i != tornadoChildren.end(); ++i) {
				// ���݂�i�̗���������
				auto tathumaki = *i;
				auto tathumakiPosition = XMVectorSet(
					tathumaki->mTransform->Position().x, 0.0f,
					tathumaki->mTransform->Position().z, 0.0f);
				auto parentPosition = XMVectorSet(
					mParentObj->mTransform->Position().x, 0.0f,
					mParentObj->mTransform->Position().z, 0.0f);
				// i�ɓ����Ă��闳���̋������v�Z����
				auto tornadoDistance = XMVector3Length(tathumakiPosition - parentPosition);
				// �ŒZ�����̗���������������
				if (tornadoDistance.x <= mTornadoMinDistance) {
					mTornadoMinDistance = tornadoDistance.x;
					mTornadoObj = tathumaki;
				}
			}
			// �͈͓��Ȃ痳���Ɋ������܂��s�����s��
			if (mTornadoMinDistance <= mTornadoDistance)
				mEnemyState = EnemyState::TornadoEscape;
			else mEnemyState = EnemyState::PlayerSearch;
		}
		else {
			// �������Ȃ��Ȃ�ʏ�s��������
			if (mEnemyState != EnemyState::DeadMove) {
				mTornadoObj = nullptr;
				mTornadoMinDistance = mTornadoDistance;
			}
		}
	}
}

// �G�̎��S�s���ł�
void Enemy::DeadMove() {
	// ���S�s���̏��
	switch (mEnemyDeadState)
	{
	case EnemyDeadState::KnockBackDead: Enemy::KnockBack(); break;
	case EnemyDeadState::BlowAwayDead: Enemy::DeadBlowAwayMove(); break;
	case EnemyDeadState::TornadoDead: Enemy::DeadTornadoBlowAwayMove(); break;
	}
}

// ������ю��̎��S�s���ł�
void Enemy::DeadBlowAwayMove() {
	// �����胂�[�V����
	auto position = mParentObj->mTransform->Position();
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

// �����Ɋ������܂ꂽ���̎��S�s���ł�
void Enemy::DeadTornadoMove() {
	// �㏸
	// �X���b�N���~�߂�
	mIsMove = false;
	// �������Ȃ��Ȃ��Ă����炻�̏�Ő�����΂�
	// ���S�����ꍇ��nullptr�Q�Ƃ��o���Ȃ��̂ł������� ���A�N�Z�X�G���[�ƂȂ�
	if (mBlowAwayTornadoObj->Name() != "Tornado") mIsTornadoBlowAway = true;
	// �����̃R���C�_�[�̎擾
	auto tornadoCollider = mBlowAwayTornadoObj->GetComponent<PhysXColliderComponent>();
	auto tornadoColliderScale = tornadoCollider->GetScale();
	// �����ɉ����ĉ�]
	auto tornadoPosition = mBlowAwayTornadoObj->mTransform->Position();
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
	// ���S�A�j���[�V����
	Enemy::SetAnimationID(2);
	if (Enemy::GetAnimationTime() > 10.0f) {
		Enemy::SetAnimationTime(10.0f);
	}
}

// �����ɐ�����΂���鎞�̎��S�s���ł�
void Enemy::DeadTornadoBlowAwayMove() {
	if (!mIsTornadoBlowAway) {
		// �����ɐ�����΂��ꂽ���̃��[�V����
		Enemy::DeadTornadoMove();
	}
	else {
		auto parentPosition = mParentObj->mTransform->Position();
		mParentObj->mTransform->Position(
			parentPosition +
			(mParentObj->mTransform->Forward() +
				(mParentObj->mTransform->Up() * cos(mBlowAwayY)))
			* Enemy::GetEnemyDeltaTime(60.0f));
		if (mBlowAwayY < 3.141593f) {
			mBlowAwayY += (3.141593f / 180.0f) * Enemy::GetEnemyDeltaTime(60.0f);
			if (mBlowAwayY > 3.141593f) mBlowAwayY = 3.141593f;
		}
		// ���S����
		Enemy::Dead();
	}
}

// �G�������I�u�W�F�N�g�ɖ߂�s���ł�
void Enemy::ReturnMove() {
	// �����I�u�W�F�N�g���Ȃ������炷���ɒʏ�s���ɖ߂�
	if (mCreateEnemyObj == nullptr) {
		mEnemyState = EnemyState::PlayerSearch;
		return;
	}
	mIsMove = true;

	auto createEnemyPosition = mCreateEnemyObj->mTransform->Position();
	// �e�̃X�e�[�^�X�̎擾
	auto parentPosition = mParentObj->mTransform->Position();
	// �����I�u�W�F�N�g�Ƃ̋����̌v�Z
	auto distance = XMVector3Length(parentPosition - createEnemyPosition);
	auto createEnemyScript = mCreateEnemyObj->GetScript<CreateEnemyObj>();
	// ��苗���܂ŋ߂Â�����ʏ�s���ɖ߂�
	if (createEnemyScript->GetReturnDistance() <= distance.x) {
		// �����I�u�W�F�N�g�Ƃ̃x�N�g���̌������v�Z
		auto v = parentPosition - createEnemyPosition;
		auto angle = atan2(v.x, v.z);
		auto quaternion = XMQuaternionRotationAxis(
			mParentObj->mTransform->Up(), angle);
		// �����I�u�W�F�N�g�̕���������
		mParentObj->mTransform->Quaternion(quaternion);
		// �O���Ɉړ�
		auto forward = mParentObj->mTransform->Forward() * mSpeed * 0.01f;
		mParentObj->mTransform->Position(parentPosition - forward);
	}
	else {
		mEnemyState = EnemyState::PlayerSearch;
	}
}

// �v���C���[�{�[�g�̑ϋv�͉񕜏������s���܂�
void Enemy::PlayerHeal() {
	// �v���C���[�̑{��
	auto player = game->FindActor("Board");
	// �v���C���[�����Ȃ���Ύ��S�����D�悷��
	if (player->Name() != "Board") return;
	auto playerScript = player->GetScript<SailBoard>();
	playerScript->Damage(-mDamage);
}

// ���S�������s���܂�
void Enemy::Dead() {
	auto parentPosition = mParentObj->mTransform->Position();
	if (!mIsPlayerHeal) {
		// �񕜏���
		Enemy::PlayerHeal();
		mIsPlayerHeal = true;
		if(mEnemyDeadState != EnemyDeadState::TornadoDead)
			mEnemyManagerScript->EnemyPlaySound("heal");
	}
	else {
		// �_�ɓ������������ �܂��� ���X�|�[���ʒu�܂ŗ����������
		if (mIsCloudHit || parentPosition.y <= mResPawnHeigth || parentPosition.y >= 300.0f) {
			game->DestroyObject(mPlayerSearchObj);
			game->DestroyObject(mEnemyCGObj);
			game->DestroyObject(gameObject);
			game->DestroyObject(gameObject->mTransform->GetParent());
		}
	}
}

// �G�̍s���֐�
void Enemy::Move() {
	/*auto collider2 = gameObject->GetComponent<PhysXColliderComponent>();
	auto collider2Scale = collider2->GetScale();
	auto rayHitObj = game->PhysX()->Raycast(
		gameObject->mTransform->Up() * collider2Scale.y / 1.5f,
		gameObject->mTransform->Up() * -10.0f, 10.0f);
	if (rayHitObj) game->Debug()->Log(std::to_string(rayHitObj->mTransform->Position().y));*/
	// ���G�͈͂̐ݒ�
	auto collider = gameObject->GetComponent<PhysXColliderComponent>();
	auto colliderScale = collider->GetScale();
	PlayerSearchMode(colliderScale);
	mPlayerSearchObj->mTransform->SetParent(mParentObj);
	// ������T��
	Enemy::SearchTornado();
	//// �����͈͓̔��ɓ����Ă��Ȃ�������ʏ�s�����s��
	// �X�C�b�`���g�����s��
	switch (mEnemyState)
	{
	case EnemyState::TornadoEscape: Enemy::TornadoEscapeMove(mTornadoObj); break;
	case EnemyState::PlayerSearch: PlayerSearchMove(); break;
	case EnemyState::PlayerChase: PlayerChaseMove(); break;
	case EnemyState::PlayerShortDistance: ShortDistanceAttack(); break;
	case EnemyState::PlayerCenterDistance: CenterDistanceAttack(); break;
	case EnemyState::PlayerLongDistance: LongDistanceAttack(); break;
	case EnemyState::DeadMove: Enemy::DeadMove(); break;
	case EnemyState::ReturnMove: Enemy::ReturnMove(); break;
	}
	// ���X�|�[�����邩�̔���
	Enemy::ResPawnLine();
	// �X�e�[�^�X�̃��Z�b�g
	mEnemyManagerScript->ResetStatus();
	// �X���[�N�̌Ăяo��
	Enemy::EnemyMoveSmoke();
}

// �G�̏�Ԃ̕ύX���s���܂�
void Enemy::ChangeEnemyState(EnemyState state) {
	mEnemyState = state;
}

// �G�̎��S��Ԃ̕ύX���s���܂�
void Enemy::ChangeEnemyDeadState(EnemyDeadState state) {
	mEnemyDeadState = state;
}

// �v���C���[�̒ǐՂ𒆎~���鋗���̉��Z�ł�
void Enemy::AddPlayerChaseStopDistance(float distance) {
	mAddPlayerChaseStopDistance = distance;
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