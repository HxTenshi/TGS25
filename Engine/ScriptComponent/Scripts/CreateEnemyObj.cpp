#include "CreateEnemyObj.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Engine\DebugEngine.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void CreateEnemyObj::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void CreateEnemyObj::Start(){
	mInitCreateInterval = mCreateInterval;
	mInitCreateDelayTime = mCreateDelayTime;
}

//���t���[���Ă΂�܂�
void CreateEnemyObj::Update(){
	auto playerObj = game->FindActor("Board");
	auto playerPosition = playerObj->mTransform->Position();
	auto playerRotate = playerObj->mTransform->Rotate();
	auto thisPosition = gameObject->mTransform->Position();
	auto distance = XMVector3Length(playerPosition - thisPosition);

	if (mCreateObj == NULL) {
		mCreateObj = game->CreateActor("Assets/FlyingFish");
	}

	if (distance.z <= mReactionDistance) {

		if (mCreateInterval <= 0) {

			if (!mIsCreateObj && mCreateCount > 0) {
				// �G�I�u�W�F�̐���
				game->AddObject(mCreateObj);
				mCreateObj->mTransform->Position(thisPosition);
				mCreateObj->mTransform->Rotate(playerRotate);

				mIsCreateObj = true;
			}
			
			mCreateDelayTime--;
			
			if (mCreateDelayTime <= 0) {
				// �e�p�����[�^�̏�����
				mCreateInterval = mInitCreateInterval;
				mCreateDelayTime = mInitCreateDelayTime;
				mCreateCount--;
				mIsCreateObj = false;
			}			
		}
		else {
			mCreateInterval--;
		}
	
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void CreateEnemyObj::Finish(){

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