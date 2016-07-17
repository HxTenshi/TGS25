#include "Sail.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"
#include "Engine\DebugEngine.h"
#include "SailBoard.h"
#include"Game\Component\MaterialComponent.h"
#include"h_standard.h"
#include"PlayerManager.h"

#include"Wind.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Sail::Initialize(){
	mSailRotate = 0.00f;
	mVelocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	move = 0;
	mRotate = 0;
	isAnimation = false;
	mBirdPos = BirdPosition::RIGHT;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Sail::Start(){
	
	
}

//���t���[���Ă΂�܂�
void Sail::Update(){
	
	if (isAnimation)
	{
		SailAnimation();
	}
	else
	{
		SailRotate();
	}

	auto parent = gameObject->mTransform->GetParent()->mTransform->GetParent();
	auto forward = parent->mTransform->Forward();
	forward.y = 0;
	forward = XMVector3Normalize(forward);

	auto velocity = parent->GetComponent<PhysXComponent>()->GetForceVelocity();
	velocity.x = 0;
	velocity.z = 0;

	if (!parent->GetScript<SailBoard>()->GetIsJump() && parent->GetScript<SailBoard>()->GetIsGround())
	{
		move += MovePower() * Speed * game->DeltaTime()->GetDeltaTime();
	}
	move -= move * 0.9f * game->DeltaTime()->GetDeltaTime();
	mVelocity.y = -5 * (60 * game->DeltaTime()->GetDeltaTime());
	move = min(max(move, 0), 100);
	
	if (!parent->GetScript<SailBoard>()->GetIsJump() && parent->GetScript<SailBoard>()->GetIsGround())
	{
		parent->GetComponent<PhysXComponent>()->SetForceVelocity(velocity + (forward * move));
	}


	auto manager = game->FindActor("PlayerManager")->GetScript<PlayerManager>();
	if (!manager->IsGameStart())
	{
		parent->GetComponent<PhysXComponent>()->SetForceVelocity(XMVectorSet(0, 0, 0, 0));
	}
	if (parent->GetScript<SailBoard>()->GetHitPoint() <= 0)
	{
		parent->GetComponent<PhysXComponent>()->SetForceVelocity(XMVectorSet(0, -9.81, 0, 0));
	}

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Sail::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Sail::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Sail::OnCollideEnter(Actor* target){

}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Sail::OnCollideExit(Actor* target){
	(void)target;
}

//���̌����{�Z�C���̌����Ő��i�͂̌v�Z �Ԃ�l
float Sail::MovePower()
{

	auto wind = gameObject->mTransform->GetParent()->mTransform->GetParent()->GetScript<SailBoard>();
	if (!wind)return 0.0f;
	auto windvec = wind->GetWind();
	windvec.y = 0;

	auto saildir = gameObject->mTransform->Left();

	saildir = XMVector3Normalize(saildir);
	if (XMVector3Length(windvec).x < 0.000000001f) {
		return 0.0f;
	}
	windvec = XMVector3Normalize(windvec);

	auto rad = XMVector3Dot(windvec, saildir);

	return abs(rad.x);
}

float Sail::GetSailRotateRad()
{
	return mSailRotate;
}

float Sail::Lerp(float value1, float value2, float amount)
{
	return (value1 * (1.0f - amount)) + (value2 * amount);
}

void Sail::SailRotate()
{
	if (Input::Down(KeyCoord::Key_O) && mSailRotate > -XM_PI / 2) {
		mSailRotate -= 0.05f;
	}
	if (Input::Down(KeyCoord::Key_P) && mSailRotate < XM_PI / 2) {
		mSailRotate += 0.05f;
	}

	if ((mSailRotate > 0 && mBirdPos == BirdPosition::RIGHT)
		|| (mSailRotate < 0 && mBirdPos == BirdPosition::LEFT))
	{
		isAnimation = true;
	}

	mSailRotate += Input::Analog(PAD_DS4_Velo3Coord::Velo3_Angular).z;

	mSailRotate = min(max(mSailRotate, -XM_PI / 2), XM_PI / 2);

	if (Input::Down(PAD_DS4_KeyCoord::Button_CROSS)) mSailRotate = 0;

	auto rotatey = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), mSailRotate);
	gameObject->mTransform->Quaternion(rotatey);
}

void Sail::SailAnimation()
{
	if (mBirdPos == BirdPosition::LEFT)
	{
		if (abs(mRotate) >= 360 * (XM_PI / 180))
		{
			mBirdPos = BirdPosition::RIGHT;
			isAnimation = false;
			mRotate = 0;
		}
		else
		{
			mRotate += RotatePower;
		}
	}
	else if (mBirdPos == BirdPosition::RIGHT)
	{
		
		if (abs(mRotate) >= 360 * (XM_PI / 180))
		{
			mBirdPos = BirdPosition::LEFT;
			isAnimation = false;
			mRotate = 0;
		}
		else
		{
			mRotate -= RotatePower;
		}
	}
	gameObject->mTransform->Rotate(XMVectorSet(0, mRotate, 0, 0));
}
