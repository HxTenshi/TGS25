#include "ExPlayer.h"

#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"

ExPlayer::ExPlayer(){
	mSpeed = 1.0f;
	mMaxSpeed = 10.0f;
	mRotateSpeed = 1.0f;
	mCameraName = "";


	mCurrentSpeed = 0.0f;
	mJumpPower = 1.0f;

	mVelocity = XMVectorSet(0, 0, 0, 1);
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
	mCurrentSpeed += z * mSpeed;
	mCurrentSpeed = max(0.0f, min(mCurrentSpeed, mMaxSpeed));

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	
	x *= mRotateSpeed;
	auto quat = XMQuaternionRotationRollPitchYaw(0, x, 0);
	auto rot = gameObject->mTransform->Quaternion();
	rot = XMQuaternionMultiply(rot, quat);
	gameObject->mTransform->Quaternion(rot);
	

	if (cc->IsGround()){
		auto v = gameObject->mTransform->Forward() * mCurrentSpeed;
		mVelocity.x = v.x;
		mVelocity.z = v.z;
		mVelocity.y = 0.0f;

		if (Input::Down(KeyCoord::Key_SPACE)) {
			mVelocity += XMVectorSet(0, mJumpPower, 0, 1);
		}
	}
	mVelocity.y -= 9.81f * 3 * game->DeltaTime()->GetDeltaTime();
	cc->Move(mVelocity * game->DeltaTime()->GetDeltaTime());
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

}

//コライダーとのロスト時に呼ばれます
void ExPlayer::OnCollideExit(Actor* target){
	//if (target->Name() == "Floor"){
	//}
	//mGroundHitCount--;
}