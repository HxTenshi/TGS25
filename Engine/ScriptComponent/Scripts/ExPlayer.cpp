#include "ExPlayer.h"

#include "../h_standard.h"
#include "../h_component.h"

ExPlayer::ExPlayer(){
	mSpeed = 1.0f;
	mMaxSpeed = 10.0f;
	mRotateSpeed = 1.0f;
	mCameraName = "";

	mGroundHitCount = 0;

	mCurrentSpeed = 0.0f;
	mJumpPower = 1.0f;
}

//生成時に呼ばれます（エディター中も呼ばれます）
void ExPlayer::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ExPlayer::Start(){

}

//毎フレーム呼ばれます
void ExPlayer::Update(){


	float x = 0.0f, z = 0.0f;
	if (Input::Down(KeyCoord::Key_W)) {
		z += 1.0f;
	}
	if (Input::Down(KeyCoord::Key_S)) {
		z -= 1.0f;
	}
	if (Input::Down(KeyCoord::Key_A)) {
		x -= 1.0f;
	}
	if (Input::Down(KeyCoord::Key_D)) {
		x += 1.0f;
	}

	mCurrentSpeed -= mCurrentSpeed * 0.4f * game->DeltaTime()->GetDeltaTime();
	mCurrentSpeed += z * mSpeed * game->DeltaTime()->GetDeltaTime();
	mCurrentSpeed = max(0.0f, min(mCurrentSpeed, mMaxSpeed));

	auto physx = gameObject->GetComponent<PhysXComponent>();
	if (!physx)return;
	
	x *= mRotateSpeed;
	gameObject->mTransform->AddTorque(XMVectorSet(0, x, 0, 1), ForceMode::eVELOCITY_CHANGE);

	if (mGroundHitCount){

		auto f = gameObject->mTransform->Forward() * mCurrentSpeed;
		physx->SetForceVelocity(f);

		if (Input::Down(KeyCoord::Key_Q)) {
			physx->SetForceVelocity(XMVectorSet(0, 0, 0, 1));
		}

		if (Input::Down(KeyCoord::Key_SPACE)) {
			gameObject->mTransform->AddForce(XMVectorSet(0, mJumpPower, 0, 1), ForceMode::eIMPULSE);
		}
	}
	mGroundHitCount = 0;
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ExPlayer::Finish(){

}

//コライダーとのヒット時に呼ばれます
void ExPlayer::OnCollideBegin(Actor* target){
	//if (target->Name() == "Floor"){
	//	
	//}
	//mGroundHitCount++;
}

//コライダーとのヒット中に呼ばれます
void ExPlayer::OnCollideEnter(Actor* target){
	if (target->Name() == "Floor"){
		mGroundHitCount = 1;
	}
}

//コライダーとのロスト時に呼ばれます
void ExPlayer::OnCollideExit(Actor* target){
	//if (target->Name() == "Floor"){
	//}
	//mGroundHitCount--;
}