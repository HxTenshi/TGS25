#include "Sail.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"
#include "Engine\DebugEngine.h"
#include "SailBoard.h"
#include"Game\Component\MaterialComponent.h"

#include"Wind.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void Sail::Initialize(){
	mRotateY = 0.00f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Sail::Start(){
	
	
}

//毎フレーム呼ばれます
void Sail::Update(){

	SailRotate();

	auto parent = gameObject->mTransform->GetParent();
	parent->mTransform->AddForce(parent->mTransform->Forward() * MovePower() * 15);        //PS4
	//parent->mTransform->AddForce(parent->mTransform->Forward() * MovePower() * 5);         //PC

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Sail::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Sail::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void Sail::OnCollideEnter(Actor* target){

}

//コライダーとのロスト時に呼ばれます
void Sail::OnCollideExit(Actor* target){
	(void)target;
}

//風の向き＋セイルで推進力の計算
float Sail::MovePower()
{

	auto wind = gameObject->mTransform->GetParent()->GetScript<SailBoard>();
	if (!wind)return 0.0f;
	auto mWindvec = wind->mWindVector;
	mWindvec.y = 0;

	auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), mRotateY);
	gameObject->mTransform->Quaternion(rotatey);

	auto temprotate = gameObject->mTransform->Left();

	temprotate = XMVector3Normalize(temprotate);
	if (XMVector3Length(mWindvec).x < 0.000000001f) {
		return 0.0f;
	}
		mWindvec = XMVector3Normalize(mWindvec);

	auto rad = XMVector3Dot(mWindvec, temprotate);

	return abs(rad.x);
}

void Sail::SailRotate()
{
	if (Input::Down(KeyCoord::Key_O) && mRotateY > -XM_PI / 2) {
		mRotateY -= 0.05f;
	}
	if (Input::Down(KeyCoord::Key_P) && mRotateY < XM_PI / 2) {
		mRotateY += 0.05f;
	}

	mRotateY += Input::Analog(PAD_DS4_Velo3Coord::Velo3_Angular).z;

	mRotateY = min(max(mRotateY, -XM_PI / 2), XM_PI / 2);

	if (Input::Down(PAD_DS4_KeyCoord::Button_PS)) mRotateY = 0;
}
