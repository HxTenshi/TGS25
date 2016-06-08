#include "EnemyCG.h"
#include "Enemy.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Engine\DebugEngine.h"
//#include "Game/Component/AnimationComponent.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyCG::Initialize(){
	mAnimationID = 0;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyCG::Start(){
	auto enemyObj = gameObject->mTransform->GetParent();
	mEnemyScript = enemyObj->GetScript<Enemy>();
}

//���t���[���Ă΂�܂�
void EnemyCG::Update(){
	/*if (mAnimationID != mEnemyScript->GetAnimationID()) {
		mAnimationID = mEnemyScript->GetAnimationID();
		auto animation = gameObject->GetComponent<AnimationComponent>();
		animation->ChangeAnimetion(mAnimationID);
	}*/
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void EnemyCG::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyCG::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyCG::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void EnemyCG::OnCollideExit(Actor* target){
	(void)target;
}