#include "TitleMnager.h"
#include "Input/Input.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"PhysX\IPhysXEngine.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void TitleMnager::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void TitleMnager::Start(){

}

//毎フレーム呼ばれます
void TitleMnager::Update()
{
	if (Input::Trigger(PAD_DS4_KeyCoord::Button_CIRCLE) || Input::Trigger(KeyCoord::Key_J))
	{
		game->LoadScene("./Assets/Scenes/nishiWindSurfing.scene");
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void TitleMnager::Finish(){

}

//コライダーとのヒット時に呼ばれます
void TitleMnager::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void TitleMnager::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void TitleMnager::OnCollideExit(Actor* target){
	(void)target;
}