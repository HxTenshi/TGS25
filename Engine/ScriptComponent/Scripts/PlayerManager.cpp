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
// サウンドボックスのスクリプト
#include "SoundBox.h"
#include "RetryScene.h"
#include"CCBoard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void PlayerManager::Initialize()
{
	mPoint = -1;
	mFadeAlpha = 0.0f;
	mStageNumberAlpha = 1.0f;
	mAlpha = 0.5;
	mFadeOutObj = nullptr;
	mGameStart = false;
	mIsWarning = false;
	mIsWarningSign = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
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

//毎フレーム呼ばれます
void PlayerManager::Update(){

	if (!mGameStart)
	{
		GameStart();
	}


	if (GameEnd())
	{
		GameOver();
	}
	else if (IsClear())
	{
		GameClear();
		
	}

	// 体力が一定以下なら再生
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

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void PlayerManager::Finish(){

}

//コライダーとのヒット時に呼ばれます
void PlayerManager::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void PlayerManager::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
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
	// ステージ名の表示
	if (mStageNumberObj == nullptr) {
		auto name = "Assets/UIPrefab/" + mStageName + ".json";
		auto stageName = name.c_str();
		mStageNumberObj = game->CreateActor(stageName);
		game->AddObject(mStageNumberObj);
		mStageNumberObj->mTransform->SetParent(gameObject);
	}
	mFadeOutObj = game->FindActor("Fade");
	if (mFadeOutObj == nullptr) {
		mFadeOutObj = game->CreateActor("Assets/Fade");
		game->AddObject(mFadeOutObj);
	}
	auto mFadeOutScript = mFadeOutObj->GetScript<Fade>();
	mFadeOutScript->FadeIn(mFadeInSecond);
	// ステージナンバーの透明処理
	if (mFadeOutScript->GetFadeInAlpha() <= 0.3f) {
		if(mFadeAlpha == 0.0f)
			mFadeAlpha = mFadeOutScript->GetFadeInAlpha();
		auto deltaTime = game->DeltaTime()->GetDeltaTime();
		mStageNumberAlpha -= (1.0f / mFadeInSecond * deltaTime) / mFadeAlpha;
	}
	auto mate = mStageNumberObj->GetComponent<MaterialComponent>();
	auto color = XMFLOAT4(1.0f, 1.0f, 1.0f, mStageNumberAlpha);
	if (mate) mate->SetAlbedoColor(color);

	if (mFadeOutScript->IsFadeIn())
	{
		game->DestroyObject(mFadeOutObj);
		game->DestroyObject(mStageNumberObj);
		mFadeOutObj = nullptr;
		//auto startTex = game->CreateActor("Assets/UIPrefab/Start.json");
		//game->AddObject(startTex);
		// サウンドボックスの生成
		StageSoundBox("start");
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
		// サウンドボックスの生成
		StageSoundBox("clear");
	}
	else
	{
		mTitleChangeTime -= game->DeltaTime()->GetDeltaTime();
		if (mTitleChangeTime < 0)
		{
			// 一度だけ生成
			if (mFadeOutObj == nullptr) {
				mFadeOutObj = game->CreateActor("Assets/Fade");
				game->AddObject(mFadeOutObj);
			}
			auto mFadeOutScript = mFadeOutObj->GetScript<Fade>();
			mFadeOutScript->FadeOut(mFadeOutSecond);
			// フェードアウト後シーン移動
			auto sceneName = "./Assets/Scenes/RetryScene.scene";
			if (mFadeOutScript->IsFadeOut()) {
				// 何も入力されていなかったらタイトルに戻るようにする
				if (mNextStageName == "") mNextStageName = "Title";
				// リスタートオブジェの生成
				auto retryObj = game->CreateActor("Assets/RetrySceneObj");
				game->AddObject(retryObj);
				retryObj->mTransform->SetParent(gameObject);
				auto retryScript = retryObj->GetScript<RetryScene>();
				// リスタートするシーンの名前を入れる
				retryScript->SetRetrySceneName(mNextStageName);
				game->LoadScene(sceneName);
			}
		}
	}
}

void PlayerManager::GameOver()
{
	// 一度だけ生成
	if (mFadeOutObj == nullptr)
	{
		mFadeOutObj = game->CreateActor("Assets/Fade");
		game->AddObject(mFadeOutObj);
	}
	auto mFadeOutScript = mFadeOutObj->GetScript<Fade>();
	mFadeOutScript->FadeOut(mFadeOutSecond, mAlpha);
	// フェードアウト後シーン移動
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
			// サウンドボックスの生成
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

// サウンドボックスを再生します
void PlayerManager::StageSoundBox(const std::string name) {
	if (mStageSoundBox == nullptr) {
		mStageSoundBox = game->CreateActor("Assets/SoundBox.json");
		game->AddObject(mStageSoundBox);
	}
	PlaySoundBox(mStageSoundBox, name);
}

// サウンドボックスを再生します(プレイヤー)
void PlayerManager::PlayerSoundBox(const std::string name) {
	if (mPlayerSoundBox == nullptr) {
		mPlayerSoundBox = game->CreateActor("Assets/SoundBox.json");
		game->AddObject(mPlayerSoundBox);
	}
	PlaySoundBox(mPlayerSoundBox, name);
}

// 指定したサウンドボックスを再生します
void PlayerManager::PlaySoundBox(Actor* soundBox, std::string name) {
	auto soundBoxScript = soundBox->GetScript<SoundBox>();
	soundBoxScript->SetSoundName(name);
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
