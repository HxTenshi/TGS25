#include "EnemyCG.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
//#include "Game/Component/AnimationComponent.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyCG::Initialize(){
	mAnimationID = 0;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyCG::Start(){

}

//���t���[���Ă΂�܂�
void EnemyCG::Update(){
	//game->DestroyObject(gameObject);

	/*auto animation = gameObject->GetComponent<AnimationComponent>();
	animation->ChangeAnimetion(mAnimationID);*/
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

void EnemyCG::ChangeAnimation(int id) {

}