#include "ParentObj.h"
#include "Enemy.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Engine\DebugEngine.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ParentObj::Initialize(){
	mCreateCount = 0;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ParentObj::Start(){
}

//���t���[���Ă΂�܂�
void ParentObj::Update(){
	if (mCreateCount == 0) {
		// �q�̐��� �����̖��O����q��I�����������܂�
		std::string baseName = "Assets/Enemy/EnemyObj/" + gameObject->Name();
		baseName.erase(baseName.end() - 9, baseName.end());
		auto createObjName = baseName.c_str();
		// �q�ł���G�̐���
		auto enemyObj = game->CreateActor(createObjName);
		game->AddObject(enemyObj);
		enemyObj->mTransform->SetParent(gameObject);

		mCreateCount = 1;
	}

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ParentObj::Finish(){
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ParentObj::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ParentObj::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void ParentObj::OnCollideExit(Actor* target){
	(void)target;
}

void ParentObj::SetPosition(float x, float z) {
	mPositionX = x;
	mPositionZ = z;
}
