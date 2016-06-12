#include "EnemyCG.h"

//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyCG::Initialize(){
	mCurrentAnimationID = 0;
	mPastAnimationID = mCurrentAnimationID;
	mAnimationTimeScale = 1.0f;
	mIsAnimationLoop = true;
	mIsChangeStatus = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyCG::Start(){
	//mCurrentAnimationID = mEnemyScript->GetAnimationID();
	//game->Debug()->Log(std::to_string(mCurrentAnimationID));
}

//���t���[���Ă΂�܂�
void EnemyCG::Update(){

	auto animation = gameObject->GetComponent<AnimationComponent>();

	// ID��������ꍇ�͕ύX�������s��
	if (mCurrentAnimationID != mPastAnimationID) {
		// �A�j���[�V������ύX
		animation->ChangeAnimetion(mCurrentAnimationID);
		// ���̃A�j���[�V�����̐ݒ��ύX����
		// ����ԂłP�E�O�̕ύX�ɂ���
		auto currentAnimaParam = animation->GetAnimetionParam(mCurrentAnimationID);
		currentAnimaParam.mWeight = 1.0f;
		currentAnimaParam.mTime = 0.0f;
		currentAnimaParam.mTimeScale = 1.0f;
		// �O�Ɏg���Ă����A�j���[�V�����̃X�e�[�^�X�����Z�b�g����
		auto pastAnimation = animation->GetAnimetionParam(mPastAnimationID);
		pastAnimation.mWeight = 0.0f;
		pastAnimation.mTime = 0.0f;
		pastAnimation.mTimeScale = 1.0f;
		// �ݒ肵���X�e�[�^�X���Z�b�g����
		animation->SetAnimetionParam(mCurrentAnimationID, currentAnimaParam);
		animation->SetAnimetionParam(mPastAnimationID, pastAnimation);

		mPastAnimationID = mCurrentAnimationID;
	}

	auto animaParam = animation->GetAnimetionParam(mCurrentAnimationID);
	// �X�e�[�^�X�ɕύX���������ꍇ�͈�񂾂��ς���
	if (mIsChangeStatus) {
		auto animaParam = animation->GetAnimetionParam(mCurrentAnimationID);
		animaParam.mTimeScale = mAnimationTimeScale;
		animaParam.mLoop = mIsAnimationLoop;
		animation->SetAnimetionParam(mCurrentAnimationID, animaParam);
		// �X�e�[�^�X��ς�����A�����ĕς��Ȃ��悤�ɂ���
		mIsChangeStatus = false;
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void EnemyCG::Finish(){
	mCurrentAnimationID = 0;
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

// �A�j���[�V������ID��ύX���܂�
void EnemyCG::SetAnimationID(int id) {
	if (mCurrentAnimationID != id) {
		mCurrentAnimationID = id;
		mIsChangeStatus = true;
	}
}

// �A�j���[�V�����̃^�C����ύX���܂�
void EnemyCG::SetAnimationTime(float time) {
	auto animation = gameObject->GetComponent<AnimationComponent>();
	auto currentAnimaParam = animation->GetAnimetionParam(mCurrentAnimationID);
	currentAnimaParam.mTime = time;
	animation->SetAnimetionParam(mCurrentAnimationID, currentAnimaParam);
}

// �A�j���[�V�����̃^�C���X�P�[����ύX���܂�
void EnemyCG::SetTimeScale(float timeScale) {
	if (mAnimationTimeScale != timeScale) {
		mAnimationTimeScale = timeScale;
		mIsChangeStatus = true;
	}
}

// �A�j���[�V�����̃��[�v��ύX���܂�
void EnemyCG::SetLoop(bool isLoop) {
	if (mIsAnimationLoop != isLoop) {
		mIsAnimationLoop = isLoop;
		mIsChangeStatus = true;
	}
}

// ���݂̃A�j���[�V�����̃^�C�����擾���܂�
float EnemyCG::GetAnimationTime() {
	auto animation = gameObject->GetComponent<AnimationComponent>();
	auto currentAnimaParam = animation->GetAnimetionParam(mCurrentAnimationID);
	return currentAnimaParam.mTime;
}