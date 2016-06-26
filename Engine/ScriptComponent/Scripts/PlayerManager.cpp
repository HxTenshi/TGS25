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
// �T�E���h�{�b�N�X�̃X�N���v�g
#include "SoundBox.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PlayerManager::Initialize()
{
	mPoint = -1;
	mAlpha = 0.5;
	mFadeOutObj = nullptr;
	mGameStart = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PlayerManager::Start()
{
	for (int i = 0 ; i < mMaxPoint; i++)
	{
		auto haneTex = game->CreateActor("Assets/UIPrefab/haneUI.json");
		game->AddObject(haneTex);
		haneTex->mTransform->Position(XMVectorSet(300 + (70 * i), 630, 0, 0));
		haneTex->mTransform->Scale(XMVectorSet(100, 100, 0, 0));
		haneTex->mTransform->SetParent(gameObject);
	}
}

//���t���[���Ă΂�܂�
void PlayerManager::Update(){

	if (!mGameStart)
	{
		GameStart();
	}


	if (mCredit <= 0)
	{
		GameOver();
	}
	else if (IsClear())
	{
		GameClear();
		
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
	
		// ��x��������
		if (mFadeOutObj == nullptr) {
			mFadeOutObj = game->FindActor("Fade");
		}
		auto mFadeOutScript = mFadeOutObj->GetScript<Fade>();
		mFadeOutScript->FadeIn(mFadeInSecond);
		if (mFadeOutScript->IsFadeIn())
		{
			game->DestroyObject(mFadeOutObj);
			mFadeOutObj = nullptr;
			auto startTex = game->CreateActor("Assets/UIPrefab/Start.json");
			game->AddObject(startTex);
			// �T�E���h�{�b�N�X�̐���
			CreateSoundBox("start");
			mGameStart = true;
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
		CreateSoundBox("clear");
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
			if (mFadeOutScript->IsFadeOut()) game->LoadScene("./Assets/Scenes/Title.scene");
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
			CreateSoundBox("over");
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

// �T�E���h�{�b�N�X�����֐��ł�
void PlayerManager::CreateSoundBox(const std::string name) {
	if (mSoundBox == nullptr) {
		mSoundBox = game->CreateActor("Assets/SoundBox.json");
		game->AddObject(mSoundBox);
	}
	auto soundBoxScript = mSoundBox->GetScript<SoundBox>();
	soundBoxScript->SetSoundName(name);
}

bool PlayerManager::IsClear()
{
	return (mPoint >= mMaxPoint - 1);
}
