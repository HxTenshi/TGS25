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
	mChangeStatus = false;
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
		// �e�A�j���[�V������Weight��ύX
		// ����ԂłP�E�O�̕ύX�ɂ���
		auto nowAnimaParam = animation->GetAnimetionParam(mCurrentAnimationID);
		nowAnimaParam.mWeight = 1.0f;
		nowAnimaParam.mTime = 0.0f;
		nowAnimaParam.mTimeScale = 1.0f;

		auto pastAnimation = animation->GetAnimetionParam(mPastAnimationID);
		pastAnimation.mWeight = 0.0f;
		pastAnimation.mTime = 0.0f;
		pastAnimation.mTimeScale = 1.0f;

		//auto aParam = gameObject->GetComponent<AnimeParam>();

		// �ݒ肵���X�e�[�^�X���Z�b�g����
		animation->SetAnimetionParam(mCurrentAnimationID, nowAnimaParam);
		animation->SetAnimetionParam(mPastAnimationID, pastAnimation);

		mPastAnimationID = mCurrentAnimationID;
	}


	auto animaParam = animation->GetAnimetionParam(mCurrentAnimationID);
	// �X�e�[�^�X�ɕύX���������ꍇ�͈�񂾂��ς���
	if (mChangeStatus) {
		auto animaParam = animation->GetAnimetionParam(mCurrentAnimationID);
		animaParam.mTimeScale = mAnimationTimeScale;
		animaParam.mLoop = mIsAnimationLoop;
		//pastAnimation.mTime += 
		/*auto deltaTime = game->DeltaTime()->GetDeltaTime();
		animaParam.mTime += deltaTime;*/
		animation->SetAnimetionParam(mCurrentAnimationID, animaParam);

		mChangeStatus = false;
	}	

	//game->Debug()->Log(std::to_string(animaParam.mTime));
	//game->DeltaTime()->SetTimeScale(1.0f);
	//game->Debug()->Log(std::to_string(game->DeltaTime()->GetDeltaTime()));
	//game->Debug()->Log(std::to_string(mCurrentAnimationID));
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
		mChangeStatus = true;
	}
}

// �A�j���[�V�����̃^�C���X�P�[����ύX���܂�
void EnemyCG::SetTimeScale(float timeScale) {
	if (mAnimationTimeScale != timeScale) {
		mAnimationTimeScale = timeScale;
		mChangeStatus = true;
	}
}

// �A�j���[�V�����̃��[�v��ύX���܂�
void EnemyCG::SetLoop(bool isLoop) {
	if (mIsAnimationLoop != isLoop) {
		mIsAnimationLoop = isLoop;
		mChangeStatus = true;
	}
}