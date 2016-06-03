#include "CreateEnemyObj.h"
#include "ParentObj.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Engine\DebugEngine.h"
// �����̎g�p
//#include <random>
#include <time.h>


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void CreateEnemyObj::Initialize(){
	mIsCreateObj = false;

	/*auto createName = ("Assets/KillerWhale");
	game->Debug()->Log(createName);*/
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void CreateEnemyObj::Start(){
	mInitCreateTimeInterval = mCreateTimeInterval;
	mInitCreateDelayTime = mCreateDelayTime;

	mVectorCount = 0;
	mRandomCreateRange = 20;

	// ���݂̎����ŗ������s���悤�ɐݒ�
	srand((unsigned int) time(NULL));

	//game->Debug()->Log(createObjName);

	//mCreateObj = game->CreateActor(createObjName);
	//if (mCreateObj == NULL) {
	//	//mCreateObj = game->CreateActor("Assets/FlyingFish");
	//}
	//game->AddObject(mCreateObj);
}

//���t���[���Ă΂�܂�
void CreateEnemyObj::Update(){

	auto playerObj = game->FindActor("Board");
	auto playerPosition = playerObj->mTransform->Position();
	auto playerRotate = playerObj->mTransform->Rotate();
	auto thisPosition = gameObject->mTransform->Position();
	auto distance = XMVector3Length(playerPosition - thisPosition);

	if (distance.z <= mReactionDistance && mCreateCount > 0) {

		if (mCreateTimeInterval <= 0) {

			if (!mIsCreateObj) {
				// �G�I�u�W�F�̐���
				//mCreateObj = game->CreateActor(createObjName);
				////if (mCreateObj == NULL) {
				////	//mCreateObj = game->CreateActor("Assets/FlyingFish");
				////}
				//game->AddObject(mCreateObj);

				/*std::random_device random;
				std::mt19937 mt(random);

				std::uniform_int_distribution<int> rand(0, 200);

				game->Debug()->Log(std::to_string(rand(mt)));*/
				game->Debug()->Log(std::to_string(GetRandom(-mRandomCreateRange, mRandomCreateRange)));
				game->Debug()->Log(std::to_string(GetRandom(-mRandomCreateRange, mRandomCreateRange)));

				// �����I�u�W�F�̖��O�w��
				// ���̃X�N���v�g�����I�u�W�F�̖��O���画�f���܂�
				baseStr = "Assets/Enemy/EnemyParentObj/";
				std::string createStr = baseStr + gameObject->Name();
				createStr.erase(createStr.end() - 9, createStr.end());
				createStr = createStr + "ParentObj";
				// string�^��char*�ɕϊ�
				createObjName = createStr.c_str();

				auto createObj = game->CreateActor(createObjName);
				// �����I�u�W�F�̎q���ɐݒ�(�I�����ɍ폜���邽��)
				//createObj->mTransform->SetParent(gameObject);

				game->AddObject(createObj);
				createObj->mTransform->Position(gameObject->mTransform->Position());
				//auto thisPositoin = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				//createObj->mTransform->Position(thisPosition);

				
				mCreateCount--;

				mIsCreateObj = true;
			}
			
			mCreateDelayTime--;
			
			if (mCreateDelayTime <= 0) {
				// �e�p�����[�^�̏�����
				mCreateTimeInterval = mInitCreateTimeInterval;
				mCreateDelayTime = mInitCreateDelayTime;
				mIsCreateObj = false;
			}			
		}
		else {
			mCreateTimeInterval--;
		}
	
	}

	if (mCreateCount <= 0) {
		auto material = gameObject->GetComponent<MaterialComponent>();
		auto color = XMFLOAT4(0, 0, 1, 1);

		material->SetAlbedoColor(color);
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void CreateEnemyObj::Finish(){
	//gameObject->mTransform->Children().clear();
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


int CreateEnemyObj::GetRandom(int min, int max) {
	// �͈͗��������c�炵���ł�
	return min + (int)(rand() * (max - min + 1.0f) / (1.0f + RAND_MAX));
}