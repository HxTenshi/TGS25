#include "ParentObj.h"
#include "Enemy.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Engine\DebugEngine.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ParentObj::Initialize(){
	/*auto childrenObj = gameObject->mTransform->Children();
	mEnemyScript = childrenObj.*/

	//mCreateCount = gameObject->mTransform->Children().size();
	mCreateCount = 0;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ParentObj::Start(){
	// �e�������ꍇ�͈ʒu�𒲐�����
	if (gameObject->mTransform->GetParent() != nullptr) {
		auto initPosi = XMVectorSet(mPositionX, 0.0f, mPositionZ, 0.0f);
		gameObject->mTransform->Position(initPosi);
	}
}

//���t���[���Ă΂�܂�
void ParentObj::Update(){
	if (mCreateCount == 0) {
		//baseStr = "Assets/";
		//std::string createStr = baseStr + gameObject->Name();
		//createStr.erase(createStr.end() - 9, createStr.end());
		//// string�^��char*�ɕϊ�
		//createObjName = createStr.c_str();
		//game->Debug()->Log(createObjName);

		// �q�̐��� �����̖��O����q��I�����������܂�
		std::string baseName = "Assets/Enemy/EnemyObj/" + gameObject->Name();
		baseName.erase(baseName.end() - 9, baseName.end());
		auto createObjName = baseName.c_str();

		// auto parentObj = game->CreateActor("Assets/FlyingFish");
		auto enemyObj = game->CreateActor(createObjName);
		game->AddObject(enemyObj);
		enemyObj->mTransform->SetParent(gameObject);

		/*mEnemyScriptName = gameObject->Name();
		mEnemyScript = enemyObj->GetScript<Enemy>();*/

		mCreateCount = 1;
	}

	/*if (!(mEnemyScript->IsDead())) {
		game->Debug()->Log("����");
		game->DestroyObject(gameObject);
	}*/
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ParentObj::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ParentObj::OnCollideBegin(Actor* target){
	//Enemy::OnCollideBegin(target);
	mEnemyScript->OnCollideBegin(target);
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ParentObj::OnCollideEnter(Actor* target){
	//(void)target;
	mEnemyScript->OnCollideEnter(target);
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void ParentObj::OnCollideExit(Actor* target){
	//(void)target;
	mEnemyScript->OnCollideExit(target);
}

void ParentObj::SetPosition(float x, float z) {
	mPositionX = x;
	mPositionZ = z;
}
