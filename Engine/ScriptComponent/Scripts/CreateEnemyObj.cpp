#include "CreateEnemyObj.h"
#include "ParentObj.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Engine\DebugEngine.h"
// �������Ԃ̎g�p
#include <time.h>


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void CreateEnemyObj::Initialize(){
	mIsCreateObj = false;
	mBaseStr = "Assets/Enemy/EnemyParentObj/";
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void CreateEnemyObj::Start(){
	mInitCreateTimeInterval = mCreateTimeInterval;
	mInitCreateDelayTime = mCreateDelayTime;
	// ���O�R���e�i�ɒǉ�
	mNameContainer.push_back("FlyingFish");
	mNameContainer.push_back("KillerWhale");

	// ���݂̎����ŗ������s���悤�ɐݒ�
	srand((unsigned int) time(NULL));
}

//���t���[���Ă΂�܂�
void CreateEnemyObj::Update(){

	auto playerObj = game->FindActor("Board");
	auto playerPosition = playerObj->mTransform->Position();
	auto playerRotate = playerObj->mTransform->Rotate();
	auto thisPosition = gameObject->mTransform->Position();
	auto distance = XMVector3Length(playerPosition - thisPosition);
	// ��������
	if (mIsInfinityCreate) mCreateCount = 100;
	// ����
	if (distance.z <= mReactionDistance && mCreateCount > 0) {
		// �����Ԋu���O�ŁA�����I�u�W�F�N�g������ꍇ��������
		if (mCreateTimeInterval <= 0) {

			if (!mIsCreateObj) {
				// ��������
				for (auto i = 0; i != mOnceCreateCount; i++) {
					CreateName();
					// �G�I�u�W�F�̐���
					auto createObj = game->CreateActor(mCreateObjName);
					// ���������Ȃ������琶�����Ȃ�
					//if (createObj == nullptr) return;
					game->AddObject(createObj);
					// �����I�u�W�F�̎q���ɐݒ�(�I�����ɍ폜�Ȃǂ����邽��)
					createObj->mTransform->SetParent(gameObject);
					// �q���ɒǉ�����̂Œl�̉��Z���s��Ȃ�
					// �����_���ňʒu��������
					auto enemyPosition = XMVectorSet(
						GetRandom(-mRandomCreateRange, mRandomCreateRange) / 10.0f,
						GetRandom(-5, 20) / 10.0f,
						GetRandom(-mRandomCreateRange, mRandomCreateRange) / 10.0f,
						0.0f);
					// �ʒu�̕ύX
					createObj->mTransform->Position(enemyPosition);
				}
				game->Debug()->Log("����");
				// �����J�E���g�̌��Z
				mCreateCount--;
				mIsCreateObj = true;
			}
			// �f�B���C�^�C���̌��Z
			mCreateDelayTime--;
			if (mCreateDelayTime <= 0) {
				// �e�p�����[�^�̏�����
				mCreateTimeInterval = mInitCreateTimeInterval;
				mCreateDelayTime = mInitCreateDelayTime;
				mIsCreateObj = false;
			}			
		}
		else {
			// �������Ԃ̌��Z
			mCreateTimeInterval--;
		}
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void CreateEnemyObj::Finish(){
	gameObject->mTransform->AllChildrenDestroy();
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void CreateEnemyObj::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void CreateEnemyObj::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void CreateEnemyObj::OnCollideExit(Actor* target){
	(void)target;
}

// �����I�u�W�F�N�g�̖��O��ݒ肵�܂�
void CreateEnemyObj::CreateName() {
	// �����I�u�W�F�̖��O�w��
	std::string createStr;
	if (!mIsRandom) {
		// ���̃X�N���v�g�����I�u�W�F�̖��O���画�f���܂�
		if(mEnemyName == "") createStr = mBaseStr + gameObject->Name();
		else createStr = mBaseStr + mEnemyName;
		createStr.erase(createStr.end() - 9, createStr.end());
	}	// �����_��
	else createStr = mBaseStr + mNameContainer[GetRandom(0, 1)];
	createStr = createStr + "ParentObj";
	// string�^��char*�ɕϊ�
	mCreateObjName = createStr.c_str();
}

// �����_���֐��ł�
int CreateEnemyObj::GetRandom(int min, int max) {
	// �͈͗��������c�炵���ł�
	return min + (int)(rand() * (max - min + 1.0f) / (1.0f + RAND_MAX));
}

// �G�������I�u�W�F�N�g�ɖ߂鋗����Ԃ��܂�
float CreateEnemyObj::GetReturnDistance() {
	return mEnemyReturnDistance;
}