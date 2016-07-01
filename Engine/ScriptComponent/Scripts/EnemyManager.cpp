#include "EnemyManager.h"
//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"
#include "Engine\DebugEngine.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyManager::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyManager::Start(){
	// �e�ł���G�̎擾
	mEnemyObj = gameObject->mTransform->GetParent();
	mEnemyParentObj = mEnemyObj->mTransform->GetParent();
	mInitParentPosition = mEnemyParentObj->mTransform->Position();
	mInitParentRotate = mEnemyParentObj->mTransform->Rotate();
}

//���t���[���Ă΂�܂�
void EnemyManager::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void EnemyManager::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyManager::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyManager::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void EnemyManager::OnCollideExit(Actor* target){
	(void)target;
}

// �G�̖ڂ̑O�ɃI�u�W�F�𐶐����܂�(�G�̐e�ɒu���ꍇ)
void EnemyManager::SetParentForwardObj(Actor* setObj) {
	//auto enemyParent = mEnemyObj->mTransform->GetParent();
	auto parentPosition = mEnemyParentObj->mTransform->Position();
	auto parentRotate = mEnemyParentObj->mTransform->Rotate();
	auto collider = mEnemyObj->GetComponent<PhysXColliderComponent>();
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

// �e�̃X�e�[�^�X�̏����������܂�
void EnemyManager::InitStatus() {
	mEnemyParentObj->mTransform->Position(mInitParentPosition);
	mEnemyParentObj->mTransform->Rotate(mInitParentRotate);
}

// �G�̃X�e�[�^�X�̃��Z�b�g���܂�
void EnemyManager::ResetStatus() {
	auto ZeroStatus = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	// �O�̒l�ɕς���
	mEnemyObj->mTransform->Position(ZeroStatus);
	mEnemyObj->mTransform->Rotate(ZeroStatus);
}

// �T�E���h���Đ����܂�
void EnemyManager::EnemyPlaySound(const std::string soundName) {
	std::string playSoundName = "Assets/Enemy/" + soundName + ".wav";
	// �T�E���h��炷
	auto sound = gameObject->GetComponent<SoundComponent>();
	if (!sound) return;
	sound->LoadFile(playSoundName);
	sound->Play();
}

// �v���C���[�Ǝw�肳�ꂽ�I�u�W�F�̈ʒu�Ƃ̋������v�Z���ĕԂ��܂�
float EnemyManager::GetPlayerDistance(Actor* playerObj, Actor* otherObj) {
	auto playerPosition = playerObj->mTransform->Position();
	auto otherPosition = otherObj->mTransform->Position();
	// �v���C���[�Ƃ̋������v�Z
	auto distance = XMVector3Length(playerPosition - otherPosition);
	return distance.z;
}

// �f���^�^�C�����擾���܂�
float EnemyManager::GetEnemyDeltaTime(float framerate) {
	auto deltaTime = game->DeltaTime()->GetDeltaTime();
	return deltaTime * framerate;
}