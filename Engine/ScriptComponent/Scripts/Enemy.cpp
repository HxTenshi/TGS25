#include "Enemy.h"
#include "PlayerSearch.h"
#include "SailBoard.h"
#include "ParentObj.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Game/Component/PhysXColliderComponent.h"
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
	mSpeed = 1.0f * 0.01f;
	mSize = gameObject->mTransform->Scale();
	mHalfSizeZ = mSize.z / 2.0f;
	mPositionY = gameObject->mTransform->Position().y;
	mRotateY = 0.0f;
	mScalarX = 1.0f;
	mScalarY = 2.0f;
	mScalarZ = 3.0f;
	mAddPlayerChaseStopDistance = 0.0f;
	mIsFloorHit = false;
	mIsImmortalBody = false;
	mIsDistanceAct = false;
	mIsAttckMode = false;
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

		// �_���[�W��^���Ď���������(���ݒ�)
		playerScript->Damage(mDamage);
		mIsDead = true;

		game->DestroyObject(mPlayerSearchObj);
		game->DestroyObject(gameObject);
		game->DestroyObject(gameObject->mTransform->GetParent());

		/*auto houkou = gameObject->mTransform->Position() - target->mTransform->Position();
		mKnockBackHoukou = XMVector3Normalize(houkou);*/
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

		mAddSearchObjCount = 1;
	}

	// ���G�I�u�W�F�̈ʒu�X�V
	mPlayerSearchObj->mTransform->Position(-mSearchObjPosition);
	//if (searchScript->IsPlayerSearch()|| mIsAttckMode)

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
					//game->Debug()->Log(std::to_string(playerDistanceNumber));
				}

				//game->Debug()->Log(std::to_string(playerDistanceNumber));

				// �G�̒ǐՍs���̌���
				mEnemyState = mDistanceVector[playerDistanceNumber];
			}
		}
	}
	else {
		mEnemyState = EnemyState::PlayerSearch;
	}
}

// �v���C���[�̕����ɉ�]���܂�
void  Enemy::PlayerChaseMode() {
	// �v���C���[�̑{��
	// �v���C���[�̕���������
	auto playerObj = game->FindActor("Board");
	auto playerPosition = playerObj->mTransform->Position();
	// �e�̃X�e�[�^�X�̎擾
	auto parentPosition = mParentObj->mTransform->Position();
	
	auto v = parentPosition - playerPosition;
	auto angle = atan2(v.x, v.z);

	auto quaternion = XMQuaternionRotationAxis(mParentObj->mTransform->Up(), angle);
	auto nomaQua = XMQuaternionNormalize(quaternion);
	//mParentObj->mTransform->Quaternion(quaternion);
	auto playerLength = XMVector3Length(playerPosition - parentPosition);

	if (abs(parentPosition.x) >= 0.0001f) {
		//if (playerLength.z > playerObj->mTransform->Scale().z) {
			mParentObj->mTransform->Quaternion(nomaQua);
			//game->Debug()->Log("��]");
		//}
	}
	
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

// �v���C���[�ɓ���������m�b�N�o�b�N���܂�
void Enemy::KnockBack() {
	gameObject->mTransform->AddForce(mKnockBackHoukou * 10.0f);
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

// ��苗���܂ŗ������烊�X�|�[�����܂�
void Enemy::ResPawnLine() {
	// ���̈ʒu�܂ŗ������烊�X�|�[���^�C�������Z
	if (mParentObj->mTransform->Position().y <= mResPawnHeigth) {

		if (mResPawnTime < 0) {
			mParentObj->mTransform->Position(mInitPosition);
			mParentObj->mTransform->Rotate(mInitRotate);
			mResPawnTime = mInitResPawnTime;
		}

		mResPawnTime--;
	}
}

// �G�̍s���֐�
void Enemy::Move() {

	// �e�̎擾	
	//mParentObj = gameObject->mTransform->GetParent();
	//game->Debug()->Log(mParentObj->Name());

	auto collider = gameObject->GetComponent<PhysXColliderComponent>();
	auto colliderScale = collider->GetScale();
	//PlayerSearchMode(gameObject->mTransform->Scale());
	PlayerSearchMode(colliderScale);
	mPlayerSearchObj->mTransform->SetParent(mParentObj);

	//gameObject->mTransform->SetParent(mParentObj);

	if (mEnemyState == EnemyState::PlayerSearch) {
		SearchMove();
	}
	else {
		if (!mIsDistanceAct) {
			if (mEnemyState == EnemyState::PlayerChase) {
				PlayerChase();
				//PlayerChaseMode();
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

	Enemy::PlayerColorChange();
	Enemy::ResPawnLine();
	

	//else {
	//	// �e�̍쐬
	//	/*auto parentObj = game->CreateActor("Assets/ParentObj");
	//	game->AddObject(parentObj);
	//	gameObject->mTransform->SetParent(parentObj);*/

	//	mParentObj = gameObject->mTransform->GetParent();
	//	mParentObj->mTransform->Position(gameObject->mTransform->Position());
	//	mParentObj->mTransform->Rotate(gameObject->mTransform->Rotate());
	//	//mParentObj = gameObject->mTransform->GetParent();
	//	//mInitParentPositionY = mParentObj->mTransform->Position().y;
	//	// �����X�e�[�^�X������
	//	mInitPosition = gameObject->mTransform->Position();
	//	mInitRotate = gameObject->mTransform->Rotate();
	//	// �q�̃X�e�[�^�X�����Z�b�g
	//	Enemy::ResetStatus();
	//	// CG�̐���
	//	//Enemy::EnemyCGCreate();
	//	game->Debug()->Log("�e����");

	//	mParentCreateCount = 1;
	//}

	
}

// �G�O���t�B�b�N�̐���
void Enemy::EnemyCGCreate() {
	//auto model = mEnemyCGObj->GetComponent<ModelComponent>();

	std::string createCGName = "Assets/Enemy/EnemyCGObj/" + gameObject->Name() + "CG";
	auto cgName = createCGName.c_str();

	mEnemyCGObj = game->CreateActor(cgName);
	game->AddObject(mEnemyCGObj);
	mEnemyCGObj->mTransform->SetParent(gameObject);
	/*game->AddObject(mEnemyCGObj);
	auto setCGScale = XMVectorSet(sizeX, sizeY, sizeZ, 0.0f);
	mEnemyCGObj->mTransform->Scale(setCGScale);
	mEnemyCGObj->mTransform->SetParent(gameObject);*/
}

// �G�̃X�e�[�^�X�̏�����
void Enemy::InitStatus() {
	gameObject->mTransform->Position(mInitPosition);
	gameObject->mTransform->Rotate(mInitRotate);
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