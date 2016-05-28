#include "HPGauge.h"

#include "Input/Input.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"PhysX\IPhysXEngine.h"


#include"SailBoard.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void HPGauge::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void HPGauge::Start(){

}

//毎フレーム呼ばれます
void HPGauge::Update()
{
	
	//横幅 = (残りHP / HP最大値) * 外枠の横幅
	auto playerScript = game->FindActor("Board")->GetScript<SailBoard>();
	if (playerScript)
	{
		auto height = (playerScript->GetHitPoint() / 100.0f) * 660;
		gameObject->mTransform->Scale(XMVectorSet(20, height, 1, 1));
		gameObject->mTransform->Position(XMVectorSet(1090, 740 - (height / 2), 1, 1));
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void HPGauge::Finish(){

}

//コライダーとのヒット時に呼ばれます
void HPGauge::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void HPGauge::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void HPGauge::OnCollideExit(Actor* target){
	(void)target;
}