#include "RainbowShiningWing.h"
//�A�N�^�[�ȂǊ�{�̃C���N���[�h
#include "h_standard.h"
//�R���|�[�l���g�S�ẴC���N���[�h
#include "h_component.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void RainbowShiningWing::Initialize(){
	mColorNumber = 0;
	mRedColor = 1.0f;
	mGreenColor = 0.0f;
	mBlueColor = 0.0f;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void RainbowShiningWing::Start(){
	// �J���[�R���e�i�ɒǉ�
	mColorContainer.push_back(mGreenColor);
	mColorContainer.push_back(mRedColor);
	mColorContainer.push_back(mBlueColor);
}

//���t���[���Ă΂�܂�
void RainbowShiningWing::Update(){
	//auto particle = gameObject->GetComponent<ParticleComponent>();
	auto deltaTime = game->DeltaTime()->GetDeltaTime();
	// 0�̏ꍇ�͉��Z�A1�̏ꍇ�͌��Z
	if (mColorNumber % 2 == 0) {
		mColorContainer[mColorNumber % 3] += 1.0f / 256 * deltaTime * (10.0f * mColorChangeSpeed);
		if (mColorContainer[mColorNumber % 3] > 1.0f) mColorContainer[mColorNumber % 3] = 1.0f;
		if (mColorContainer[mColorNumber % 3] == 1.0f) mColorNumber++;
	}
	else {
		mColorContainer[mColorNumber % 3] -= 1.0f / 256 * deltaTime * (10.0f * mColorChangeSpeed);
		if (mColorContainer[mColorNumber % 3] < 0.0f) mColorContainer[mColorNumber % 3] = 0.0f;
		if (mColorContainer[mColorNumber % 3] == 0.0f) mColorNumber++;
	}
	// �F�̕ύX
	auto material = gameObject->GetComponent<MaterialComponent>();
	auto color = XMFLOAT4(mColorContainer[1], mColorContainer[0], mColorContainer[2], mColorAlpha);
	if (material) material->SetAlbedoColor(color);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void RainbowShiningWing::Finish(){
	game->DestroyObject(gameObject);
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void RainbowShiningWing::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void RainbowShiningWing::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void RainbowShiningWing::OnCollideExit(Actor* target){
	(void)target;
}