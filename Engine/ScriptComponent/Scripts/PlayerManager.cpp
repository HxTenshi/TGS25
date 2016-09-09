#include "PlayerManager.h"

#include "Input/Input.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Game\Component\TextureModelComponent.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"PhysX\IPhysXEngine.h"
#include"Fade.h"
#include"h_standard.h"
#include"HaneEffect.h"
#include "SailBoard.h"
// �T�E���h�{�b�N�X�̃X�N���v�g
#include "SoundBox.h"
#include "RetryScene.h"
#include"CCBoard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PlayerManager::Initialize()
{
	mPoint = -1;
	mGameStartTime = 2.0f;
	mFadeAlpha = 0.0f;
	mStageNumberAlpha = mGameStartTime + 1.0f;
	mGatherAlpha = mGameStartTime + 1.0f;
	mAlpha = 0.5;
	mFadeOutObj = nullptr;
	mGameStart = false;
	mIsWarning = false;
	mIsWarningSign = false;
	mIsFont = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PlayerManager::Start()
{
	for (int i = 0 ; i < mMaxPoint; i++)
	{
		auto haneTex = game->CreateActor("Assets/UIPrefab/haneUI.json");
		game->AddObject(haneTex);
		haneTex->mTransform->Position(XMVectorSet(200 + (70 * i), 630, 0, 0));
		haneTex->mTransform->Scale(XMVectorSet(100, 100, 0, 0));
		haneTex->mTransform->SetParent(gameObject);
	}

	mFadeOutObj = game->FindActor("Fade");
	if (mFadeOutObj == nullptr) {
		mFadeOutObj = game->CreateActor("Assets/Fade");
		game->AddObject(mFadeOutObj);
	}
	
}

//���t���[���Ă΂�܂�
void PlayerManager::Update(){

	if (!mGameStart)
	{
		GameStart();
	}
	if (mIsFont) FadeFontObj();

	if (GameEnd())
	{
		GameOver();
	}
	else if (IsClear())
	{
		GameClear();
		
	}

	// �̗͂����ȉ��Ȃ�Đ�
	auto player = game->FindActor("Board");
	if (player != nullptr) {
		auto playerScript = player->GetScript<CCBoard>();
		if (playerScript->GetHitPoint() <= 25.0f) mIsWarning = true;
		else {
			mIsWarning = false;
			mIsWarningSign = true;
		}

		if (mIsWarning) {
			if (mIsWarningSign) {
				PlayerSoundBox("warning1");
				mIsWarningSign = false;
			}
		}
	}

	/*auto text1 = game->FindActor("column1")->GetComponent<TextureModelComponent>();
	auto text2 = game->FindActor("column10")->GetComponent<TextureModelComponent>();

	text1->SetTexture("Assets/num/" +std::to_string(mCredit % 10) + ".png");
	text2->SetTexture("Assets/num/" + std::to_string((mCredit / 10) % 10) + ".png");*/


	/*auto hane1 = game->FindActor("hane1")->GetComponent<TextureModelComponent>();
	auto hane2 = game->FindActor("hane2")->GetComponent<TextureModelComponent>();
	auto hane3 = game->FindActor("hane3")->GetComponent<TextureModelComponent>();

	if (mMaxPoint == 2)
	{
		hane1->SetTexture("./Assets/UI/hane.png");
		
	}
	if (mMaxPoint == 1)
	{
		hane2->SetTexture("./Assets/UI/hane.png");
	}
	if (mMaxPoint == 0)
	{
		hane3->SetTexture("./Assets/UI/hane.png");
	}*/

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void PlayerManager::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PlayerManager::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PlayerManager::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void PlayerManager::OnCollideExit(Actor* target){
	(void)target;
}

void PlayerManager::CreditDown()
{
	mCredit--;
}

void PlayerManager::CreditUp()
{
	mCredit++;
}

void PlayerManager::ItemGet()
{
	mPoint++;
	mPoint = min(max(0, mPoint), mMaxPoint - 1);
	WingUI();
}

bool PlayerManager::IsGameStart()
{
	return mGameStart;
}

void PlayerManager::GameStart()
{
	// �X�e�[�W���̕\��
	//CreateFontObj(mStageName, mStageNumberObj);
	if (mStageNumberObj == nullptr) {
		auto objname = "Assets/UIPrefab/" + mStageName + ".json";
		auto stageName = objname.c_str();
		mStageNumberObj = game->CreateActor(stageName);
		game->AddObject(mStageNumberObj);
		mStageNumberObj->mTransform->SetParent(gameObject);
		objname = "Assets/UIPrefab/RainbowGatherFont.json";
		stageName = objname.c_str();
		mGatherObj = game->CreateActor(stageName);
		game->AddObject(mGatherObj);
		mGatherObj->mTransform->SetParent(gameObject);
		//CreateFontObj(mStageName, mStageNumberObj);
		//CreateFontObj("RainbowGatherFont", mGatherObj);
	}
	mFadeOutObj = game->FindActor("Fade");
	if (mFadeOutObj == nullptr) {
		mFadeOutObj = game->CreateActor("Assets/Fade");
		game->AddObject(mFadeOutObj);
	}
	auto mFadeOutScript = mFadeOutObj->GetScript<Fade>();
	mFadeOutScript->FadeIn(mFadeInSecond);
	if (mFadeOutScript->IsFadeIn()) {
		// �T�E���h�{�b�N�X�̐���
		StageSoundBox("start");
		mGameStart = true;
		mIsFont = true;
	}
}

void PlayerManager::GameClear()
{
	auto cleartexture = game->FindActor("Clear");
	if (!cleartexture)
	{
		cleartexture = game->CreateActor("Assets/UIPrefab/Clear.json");
		game->AddObject(cleartexture);
		// �T�E���h�{�b�N�X�̐���
		StageSoundBox("clear");
	}
	else
	{
		mTitleChangeTime -= game->DeltaTime()->GetDeltaTime();
		if (mTitleChangeTime < 0)
		{
			// ��x��������
			if (mFadeOutObj == nullptr) {
				mFadeOutObj = game->CreateActor("Assets/Fade");
				game->AddObject(mFadeOutObj);
			}
			auto mFadeOutScript = mFadeOutObj->GetScript<Fade>();
			mFadeOutScript->FadeOut(mFadeOutSecond);
			// �t�F�[�h�A�E�g��V�[���ړ�
			auto sceneName = "./Assets/Scenes/RetryScene.scene";
			if (mFadeOutScript->IsFadeOut()) {
				// �������͂���Ă��Ȃ�������^�C�g���ɖ߂�悤�ɂ���
				if (mNextStageName == "") mNextStageName = "Title";
				// ���X�^�[�g�I�u�W�F�̐���
				auto retryObj = game->CreateActor("Assets/RetrySceneObj");
				game->AddObject(retryObj);
				retryObj->mTransform->SetParent(gameObject);
				auto retryScript = retryObj->GetScript<RetryScene>();
				// ���X�^�[�g����V�[���̖��O������
				retryScript->SetRetrySceneName(mNextStageName);
				game->LoadScene(sceneName);
			}
		}
	}
}

void PlayerManager::GameOver()
{
	// ��x��������
	if (mFadeOutObj == nullptr)
	{
		mFadeOutObj = game->CreateActor("Assets/Fade");
		game->AddObject(mFadeOutObj);
	}
	auto mFadeOutScript = mFadeOutObj->GetScript<Fade>();
	mFadeOutScript->FadeOut(mFadeOutSecond, mAlpha);
	// �t�F�[�h�A�E�g��V�[���ړ�
	if (mFadeOutScript->IsFadeOut() && mTitleChangeTime < 0)
	{
		game->LoadScene("./Assets/Scenes/Title.scene");
	}
	else if (mFadeOutScript->IsFadeOut())
	{
		auto cleartexture = game->FindActor("GameOver");
		if (!cleartexture)
		{
			cleartexture = game->CreateActor("Assets/UIPrefab/GameOver.json");
			game->AddObject(cleartexture);
			// �T�E���h�{�b�N�X�̐���
			StageSoundBox("over");
		}
		else
		{
			mTitleChangeTime -= game->DeltaTime()->GetDeltaTime();
			if (mTitleChangeTime < 0)
			{
				mAlpha = 1.0f;
				mFadeOutObj = game->CreateActor("Assets/Fade");
				game->AddObject(mFadeOutObj);
			}
		}
	}
}

void PlayerManager::WingUI()
{
	std::vector<Actor*> items;
	auto it = gameObject->mTransform->Children().begin();
	while (it != gameObject->mTransform->Children().end())
	{
		items.push_back(*it);
		++it;
	}
	items[mPoint]->GetComponent<TextureModelComponent>()->SetTexture("./Assets/UI/hane.png");
}

// �T�E���h�{�b�N�X���Đ����܂�
void PlayerManager::StageSoundBox(const std::string name) {
	if (mStageSoundBox == nullptr) {
		mStageSoundBox = game->CreateActor("Assets/SoundBox.json");
		game->AddObject(mStageSoundBox);
	}
	PlaySoundBox(mStageSoundBox, name);
}

// �T�E���h�{�b�N�X���Đ����܂�(�v���C���[)
void PlayerManager::PlayerSoundBox(const std::string name) {
	if (mPlayerSoundBox == nullptr) {
		mPlayerSoundBox = game->CreateActor("Assets/SoundBox.json");
		game->AddObject(mPlayerSoundBox);
	}
	PlaySoundBox(mPlayerSoundBox, name);
}

// �w�肵���T�E���h�{�b�N�X���Đ����܂�
void PlayerManager::PlaySoundBox(Actor* soundBox, std::string name) {
	auto soundBoxScript = soundBox->GetScript<SoundBox>();
	soundBoxScript->SetSoundName(name);
}

// �t�H���g�̐����֐��ł�
void PlayerManager::CreateFontObj(const std::string name, Actor* obj) {
	if (obj != nullptr) return;
	auto objname = "Assets/UIPrefab/" + name + ".json";
	auto stageName = objname.c_str();
	obj = game->CreateActor(stageName);
	game->AddObject(obj);
	obj->mTransform->SetParent(gameObject);
}

void PlayerManager::FadeFontObj() {
	// �X�e�[�W�i���o�[�̓�������
	auto deltaTime = game->DeltaTime()->GetDeltaTime();
	mStageNumberAlpha -= mFadeInSecond * deltaTime;
	mGatherAlpha -= mFadeInSecond * deltaTime;
	auto stageNmuberMate = mStageNumberObj->GetComponent<MaterialComponent>();
	auto stageNumberColor = XMFLOAT4(1.0f, 1.0f, 1.0f, mStageNumberAlpha);
	if (stageNmuberMate) stageNmuberMate->SetAlbedoColor(stageNumberColor);
	//mGatherObj
	auto gatherMate = mGatherObj->GetComponent<MaterialComponent>();
	auto gatherNumberColor = XMFLOAT4(1.0f, 1.0f, 1.0f, mGatherAlpha);
	if (gatherMate) gatherMate->SetAlbedoColor(gatherNumberColor);
	if (mStageNumberAlpha <= 0.0f)
	{
		game->DestroyObject(mStageNumberObj);
		game->DestroyObject(mGatherObj);
		game->DestroyObject(mFadeOutObj);
		mFadeOutObj = nullptr;
		mIsFont = false;
	}
}

bool PlayerManager::IsClear()
{
	return (mPoint >= mMaxPoint - 1);
}

bool PlayerManager::GameEnd()
{
	auto player = game->FindActor("Board");
	auto deadLine = game->FindActor("DeadLine");
	if (!player || !deadLine) return true;
	if (player->mTransform->Position().y <= deadLine->mTransform->Position().y)
	{
		return true;
	}
	return false;
}
