#include "PlayerManager.h"

#include "Input/Input.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include"Game\Component\TextureModelComponent.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"PhysX\IPhysXEngine.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void PlayerManager::Initialize()
{
	mCredit = 3;
	mMaxPoint = 5;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void PlayerManager::Start(){

}

//毎フレーム呼ばれます
void PlayerManager::Update(){
	if (Input::Trigger(KeyCoord::Key_SPACE)) game->Debug()->Log(std::to_string(mMaxPoint));
	if (mCredit <= 0) game->LoadScene("./Assets/Scenes/SampleBall.scene");
	if (mMaxPoint <= 0) game->LoadScene("./Assets/Scenes/SampleBall.scene");

	auto text1 = game->FindActor("column1")->GetComponent<TextureModelComponent>();
	auto text2 = game->FindActor("column10")->GetComponent<TextureModelComponent>();

	text1->SetTexture("Assets/num/" +std::to_string(mCredit % 10) + ".png");
	text2->SetTexture("Assets/num/" + std::to_string((mCredit / 10) % 10) + ".png");

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
	mMaxPoint--;
}
