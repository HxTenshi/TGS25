#include "Enemy.h"
#include "PlayerSearch.h"
#include "SailBoard.h"
#include "ParentObj.h"
#include "EnemyCG.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"

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
	mTornadoRotate = 0.0f;
	mTornadoPower = 0.0f;
	mTornadoDistance = 0.0f;
	mBlowAwayPower = 20.0f;
	mBlowAwayInterval = 4.0f;
	mIsFloorHit = false;
	mIsCloudHit = false;
	mIsImmortalBody = false;
	mIsDistanceAct = false;
	mIsBlowAway = true;
	mIsKnockBackDirection = false;
	mIsAttckMode = false;
	mIsChaseRotate = true;
	mIsTornadoRange = false;
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
	//game->Debug()->Log(mParentObj->Name());
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

		// ���S�����u�Ԃɓ����蔻����g���K�[�ɂ���
		/*auto objPhysxCollider = gameObject->GetComponent<PhysXColliderComponent>();
		objPhysxCollider->SetIsTrigger(true);*/
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
	//mParentObj = gameObject->mTransform->GetParent();

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
		//CGObj->mTransform->SetParent(gameObject);
		CGObj->mTransform->SetParent(mParentObj);
		mEnemyCGScript = CGObj->GetScript<EnemyCG>();

		mAddSearchObjCount = 1;
	}

	// ���G�I�u�W�F�̈ʒu�X�V
	mPlayerSearchObj->mTransform->Position(-mSearchObjPosition);

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
	auto playerPosition = playerObj->mTransform->Position();
	// �e�̃X�e�[�^�X�̎擾
	auto parentPosition = mParentObj->mTransform->Position();
	
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
	//auto tornadoObj = game->FindActor("Tornado");
	auto tornadoPosition = tornadoObj->mTransform->Position();
	// �e�̃X�e�[�^�X�̎擾
	auto parentPosition = mParentObj->mTransform->Position();
	// �����Ƃ̉�]�������v�Z(������悤�ɂ���)
	auto v = parentPosition - tornadoPosition;
	auto angle = atan2(v.x, v.z);
	auto escapeAngle = angle + 3.141593f / 2.0f;
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
		(mParentObj->mTransform->Forward() + (mParentObj->mTransform->Left() / 10.0f)) * (mTornadoPower * 0.01f));

	// ������ԊԊu�������i���S���[�V�����̒l��ύX�j


	Enemy::SetAnimationID(0);
}

// �v���C���[�ɓ���������m�b�N�o�b�N���܂�
void Enemy::KnockBack() {
	auto parentPosition = mParentObj->mTransform->Position();

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

	mParentObj->mTransform->Position(parentPosition + (mKnockBackDIrection * 20.0f * 0.01f) -
		mParentObj->mTransform->Up() * 5.0f * 0.01f);
	Enemy::Dead();
	/*mParentObj->mTransform->Position(
		(mParentObj->mTransform->Forward() - mParentObj->mTransform->Up())* 10.0f * 0.01f);*/
}

// �G�̖ڂ̑O�ɃI�u�W�F�𐶐����܂�(�e������ꍇ)
void Enemy::SetParentForwardObj(Actor* setObj) {
	auto parentPosition = mParentObj->mTransform->Position();
	auto parentRotate = mParentObj->mTransform->Rotate();
	//auto parentScale = gameObject->mTransform->Scale();
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

// ��苗���܂ŗ������烊�X�|�[�����܂�
void Enemy::ResPawnLine() {
	// ���̈ʒu�܂ŗ������烊�X�|�[���^�C�������Z
	if (mParentObj->mTransform->Position().y <= mResPawnHeigth) {

		if (mResPawnTime < 0) {
			Enemy::InitStatus();
			mResPawnTime = mInitResPawnTime;
			mInitSetCount = 0;
		}
		
		mResPawnTime--;
	}
}

// �G�̎��S�s���ł�
void Enemy::DeadMove() {

	auto position = mParentObj->mTransform->Position();
	// ������ԏꍇ
	if (mIsBlowAway) {
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
		mParentObj->mTransform->Position(position + up);
	}
	else {
		Enemy::KnockBack();
	}
	// �ēx�G��Ă��_���[�W����
	Enemy::SetDamage(0);
}

// ���S�������s���܂�
void Enemy::Dead() {
	auto parentPosition = mParentObj->mTransform->Position();

	if (mIsCloudHit) {
		game->DestroyObject(mPlayerSearchObj);
		game->DestroyObject(mEnemyCGObj);
		game->DestroyObject(gameObject);
		game->DestroyObject(gameObject->mTransform->GetParent());
	}
	else {
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

	// �e�̎擾	
	//mParentObj = gameObject->mTransform->GetParent();
	//game->Debug()->Log(mParentObj->Name());
	if (!mIsDead) {
		auto collider = gameObject->GetComponent<PhysXColliderComponent>();
		auto colliderScale = collider->GetScale();
		PlayerSearchMode(colliderScale);
		mPlayerSearchObj->mTransform->SetParent(mParentObj);

		if (!mIsTornadoRange) {
			if (mEnemyState == EnemyState::PlayerSearch) {
				SearchMove();
			}
			else {
				if (!mIsDistanceAct) {
					if (mEnemyState == EnemyState::PlayerChase) {
						PlayerChase();
					}
				}
				else {

					if (mEnemyState == EnemyState::PlayerShortDistance) {
						ShortDistanceAttack();
						//game->Debug()->Log("�ߋ���");
					}
					else if (mEnemyState == EnemyState::PlayerCenterDistance) {
						CenterDistanceAttack();
						//game->Debug()->Log("������");
					}
					else if (mEnemyState == EnemyState::PlayerLongDistance) {
						LongDistanceAttack();
						//game->Debug()->Log("������");
					}
				}
			}
		}
		else {
			Enemy::TornadoEscapeMove(mTornadoObj);
		}

		//Enemy::PlayerColorChange();
		Enemy::ResPawnLine();
	}
	else {
		// ���S�s���̌Ăяo��
		Enemy::DeadMove();
	}
	Enemy::ResetStatus();

	//// �����̐e����
	//auto tornadoParent = game->FindActor("TornadoParent");
	//if (tornadoParent != nullptr) {
	//	auto tornadoChildren = tornadoParent->mTransform->Children();
	//	for (int i = tornadoChildren.begin; i < tornadoChildren.end; ++i) {
	//		/*auto distance = XMVector3Length(
	//			tathumaki->mTransform->Position() - mParentObj->mTransform->Position());*/
	//	}
	//}

	// �������߂��ɂ���ꍇ
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
		// �r���ŗ������Ȃ��Ȃ�����ʏ�s���ɖ߂�
		mIsTornadoRange = false;
	}
}

// �e�̃X�e�[�^�X�̏�����
void Enemy::InitStatus() {
	mParentObj->mTransform->Position(mInitPosition);
	mParentObj->mTransform->Rotate(mInitRotate);
}

// �G�̃X�e�[�^�X�̃��Z�b�g
void Enemy::ResetStatus() {
	auto ZeroStatus = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	gameObject->mTransform->Position(ZeroStatus);
	gameObject->mTransform->Rotate(ZeroStatus);
}

// 
void Enemy::AddPlayerChaseStopDistance(float distance) {
	mAddPlayerChaseStopDistance = distance;
}

void Enemy::SetTornadoStatus(const float power, const float interval, const float distance) {
	mTornadoPower = power;
	mBlowAwayInterval = interval;
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

// �A�j���[�V�����̃^�C���X�P�[����ύX���܂�
void Enemy::SetAnimationTimeScale(float timeScale) {
	mEnemyCGScript->SetTimeScale(timeScale);
}

// �q��������A�j���[�V�����̃��[�v��ύX���܂�
void Enemy::SetAnimationLoop(bool isLoop) {
	mEnemyCGScript->SetLoop(isLoop);
}