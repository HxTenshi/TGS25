#include "Fade.h"

//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"
#include "Engine\DebugEngine.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Fade::Initialize(){
	mFadeInAlpha = 1.0f;
	mFadeOutAlpha = 0.0f;
	mIsFadeIn = false;
	mIsFadeOut = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Fade::Start(){
}

//���t���[���Ă΂�܂�
void Fade::Update(){
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Fade::Finish(){
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Fade::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Fade::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Fade::OnCollideExit(Actor* target){
	(void)target;
}

void Fade::FadeIn(const float time) {
	// �f���^�^�C���̎擾
	auto deltaTime = game->DeltaTime()->GetDeltaTime();
	mFadeInAlpha -= 1.0f / time * deltaTime;
	if (mFadeInAlpha < 0.0f) mFadeInAlpha = 0.0f;
	// �`��
	auto mate = gameObject->GetComponent<MaterialComponent>();
	auto color = XMFLOAT4(0.0f, 0.0f, 0.0f, mFadeInAlpha);
	if (mate) mate->SetAlbedoColor(color);
	if (mFadeInAlpha == 0.0f) {
		mIsFadeIn = true;
		// game->DestroyObject(gameObject);
	}
}

// 
void Fade::FadeOut(const float time) {
	// �f���^�^�C���̎擾
	auto deltaTime = game->DeltaTime()->GetDeltaTime();
	if (mFadeOutAlpha > 1.0f) mFadeOutAlpha = 1.0f;
	else if(mFadeOutAlpha < 1.0f) mFadeOutAlpha += 1.0f / time * deltaTime;
	//game->Debug()->Log(std::to_string(mFadeOutAlpha));
	//mFadeOutAlpha = 0.0f;
	// �`��
	auto mate = gameObject->GetComponent<MaterialComponent>();
	//game->Debug()->Log(std::to_string(mate->mAlbedo.w));
	auto color = XMFLOAT4(0.0f, 0.0f, 0.0f, mFadeOutAlpha);
	mate->SetAlbedoColor(color);
	//if (mate) mate->SetAlbedoColor(color);
	if (mFadeOutAlpha == 1.0f) mIsFadeOut = true;
}

// �t�F�[�h�C������������Ԃ��܂�
bool Fade::IsFadeIn() {
	return mIsFadeIn;
}

// 
bool Fade::IsFadeOut() {
	return mIsFadeOut;
}