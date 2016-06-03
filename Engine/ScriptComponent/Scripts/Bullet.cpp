#include "Bullet.h"

#include "Game/Actor.h"
#include "Game/Component/TransformComponent.h"
#include "Input/Input.h"
#include "Game/Script/IGame.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Bullet::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Bullet::Start(){

}

//���t���[���Ă΂�܂�
void Bullet::Update(){

	mDeadTime -= 0.016f;
	if (mDeadTime < 0){
		game->DestroyObject(gameObject);
	}
	auto pos = gameObject->mTransform->Position();
	auto s = gameObject->mTransform->LossyScale();
	auto vec = gameObject->mTransform->Forward() * mSpeed;
	vec.x *= s.x;
	vec.y *= s.y;
	vec.z *= s.z;
	gameObject->mTransform->Position(pos + vec);

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Bullet::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Bullet::OnCollideBegin(Actor* target){

	game->DestroyObject(gameObject);
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Bullet::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Bullet::OnCollideExit(Actor* target){
	(void)target;
}