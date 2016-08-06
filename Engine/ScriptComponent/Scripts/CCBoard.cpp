#include "CCBoard.h"
#include "Game/Component/CharacterControllerComponent.h"
#include"Game\Component\AnimationComponent.h"
#include"MoveSmoke.h"
#include"HaneEffect.h"
#include"Wind.h"
#include"PlayerManager.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void CCBoard::Initialize()
{
	mWindVector = XMVectorSet(1, 0, 0, 1);
	mState = State::STANDBY;

	//シリアライズ------------------------
	/*mSpeed = 1.0f;
	mMaxSpeed = 10.0f;
	mRotateSpeed = 1.0f;
	mCurrentSpeed = 0.0f;
	mJumpPower = 10.0f;
	mAttackSpeed = 1.0f;
	mTrickCondition = 1.0f;
	mTrickRotatePower = 0.05f;*/
	//-------------------------------------

	mVelocity = XMVectorSet(0, 0, 0, 1);

	mTrickRotate = XMVectorSet(0, 0, 0, 1);
	mTrickPoint = 0.0f;
	isJump = false;
	isTornado = false;
	mPlyerHP = 100;
	mPoint = 0;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void CCBoard::Start()
{
	mPrevAcceler = Input::Analog(PAD_DS4_Velo3Coord::Velo3_Acceleration).z;
	mArrow = game->FindActor("Arrow");
	auto mCC = gameObject->GetComponent<CharacterControllerComponent>();
	if (!mCC)return;
	mCC->Teleport(gameObject->mTransform->Position());
}

//毎フレーム呼ばれます
void CCBoard::Update()
{
	mPlyerHP -= mSlipDamage * (game->DeltaTime()->GetDeltaTime() * 60);
	mPlyerHP = min(max(mPlyerHP, 0), 100);
	CreateAttackWind();
	StateUpdate(game->DeltaTime()->GetDeltaTime());
	if (mPlyerHP <= 0)
	{
		StateChange(State::DEAD);
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void CCBoard::Finish() 
{
}

//コライダーとのヒット時に呼ばれます
void CCBoard::OnCollideBegin(Actor* target) {
	(void)target;
	if (target->Name() == "Tree" || target->Name() == "Tower")
	{
		PlaySE("Assets/PlayerSE/hit.wav");
	}

	if (target->Name() == "PointItem") {

		auto effect = game->CreateActor("Assets/Effect/haneEffect.json");
		game->AddObject(effect);
		effect->GetScript<HaneEffect>()->SetPosition(XMVectorSet(200 + (70 * mPoint), 630, 0, 0));
		mPoint++;
		PlaySE("Assets/PlayerSE/recovery.wav");
		game->DestroyObject(target);
	}
}

//コライダーとのヒット中に呼ばれます
void CCBoard::OnCollideEnter(Actor* target) {
	(void)target;
	if (mState == State::DEAD)
	{
		auto collider = target->GetComponent<PhysXColliderComponent>();
		if (!collider) return;
		collider->SetIsTrigger(true);
	}
	if (target->Name() == "Wind")
	{
		if (target->GetScript<Wind>()) mWindVector = target->GetScript<Wind>()->GetWindVelocity();
	}
}

//コライダーとのロスト時に呼ばれます
void CCBoard::OnCollideExit(Actor* target) {
	(void)target;
}

XMVECTOR CCBoard::GetWind()
{
	return mWindVector;
}

int CCBoard::GetState()
{
	return mState;
}

void CCBoard::Damage(int damage)
{
	if (damage > 0) AnimationChange(2);
	mPlyerHP -= damage;
	mPlyerHP = min(max(mPlyerHP, 0), 100);
}

bool CCBoard::IsTrick()
{
	return isTornado;
}

float CCBoard::GetHitPoint()
{
	return mPlyerHP;
}

bool CCBoard::IsUnrivaled()
{
	if (mCurrentSpeed > mAttackSpeed)
	{
		return true;
	}
	return false;
}

void CCBoard::StateUpdate(float deltaTime)
{
	//game->Debug()->Log(std::to_string(mState));
	switch (mState)
	{
	case State::STANDBY: Standby(deltaTime); break;
	case State::MOVE: Move(deltaTime); break;
	case State::JUMP: Jump(deltaTime); break;
	case State::TORNADO: Tornado(deltaTime); break;
	case State::DEAD: Dead(deltaTime); break;
	}
}

void CCBoard::Standby(float deltaTime)
{
	MoveSmokeParameterSet(0,mMaxSpeed);
	auto manager = game->FindActor("PlayerManager");
	if (!manager){ StateChange(State::MOVE); return;}
	auto script = manager->GetScript<PlayerManager>();
	if(!script) { StateChange(State::MOVE); return; }
	if (script->IsGameStart())
	{
		StateChange(State::MOVE);
	}
}

void CCBoard::Move(float deltaTime)
{
	if (mArrow == nullptr)
	{
		mArrow = game->CreateActor("Assets/board/Arrow.json");
		game->AddObject(mArrow);
	}

	auto bird = game->FindActor("Bird");
	if (!bird) return;
	auto mBirdAnimation = bird->GetComponent<AnimationComponent>();
	if (!mBirdAnimation) return;

	if ((mBirdAnimation->mCurrentSet != 2 || mBirdAnimation->GetAnimetionParam(2).mTime >= 21))
	SailRotateAnimation();

	isTornado = false;
	float x = 0.0f;
	if (Input::Down(KeyCoord::Key_A)) {
		x -= 0.1f;
	}
	if (Input::Down(KeyCoord::Key_D)) {
		x += 0.1f;
	}

	auto sail = game->FindActor("Sail");
	float windPower = 0.0f;
	if (sail)
	{
		if (sail->GetScript<CCSail>())
		{
			windPower = sail->GetScript<CCSail>()->MovePower();
		}
	}
	mCurrentSpeed -= mCurrentSpeed * 0.8f * game->DeltaTime()->GetDeltaTime();
	mCurrentSpeed += windPower * mSpeed;
	mCurrentSpeed = max(0.0f, min(mCurrentSpeed, mMaxSpeed));

	MoveSmokeParameterSet(mCurrentSpeed, mMaxSpeed);

	x *= mRotateSpeed * (mCurrentSpeed /mMaxSpeed);
	auto quat = XMQuaternionRotationRollPitchYaw(0, x, 0);
	auto rot = gameObject->mTransform->Quaternion();
	rot = XMQuaternionMultiply(rot, quat);
	gameObject->mTransform->Quaternion(rot);

	auto mCC = gameObject->GetComponent<CharacterControllerComponent>();
	if (!mCC)return;
	if (mCC->IsGround()) {
		auto v = gameObject->mTransform->Forward() * mCurrentSpeed;
		mVelocity.x = v.x;
		mVelocity.z = v.z;
		mVelocity.y = 0.0f;

		if (Input::Down(KeyCoord::Key_SPACE)) {
			isJump = true;
			mVelocity += XMVectorSet(0, mJumpPower, 0, 1);

		}
	}
	else
	{
		if (isJump) StateChange(State::JUMP);
	}

	GroundCheck();

	mVelocity.y -= 9.81f * 3 * game->DeltaTime()->GetDeltaTime();
	mCC->Move(mVelocity * game->DeltaTime()->GetDeltaTime());
	//AnimationChange(3);
}

void CCBoard::Jump(float deltaTime)
{
	if (mArrow)
	{
		game->DestroyObject(mArrow);
		mArrow = nullptr;
	}

	MoveSmokeParameterSet(0, mMaxSpeed);

	Trick(deltaTime);
	auto mCC = gameObject->GetComponent<CharacterControllerComponent>();
	if (!mCC) return;

	if (mCC->IsGround())
	{
		auto bomb = game->CreateActor("Assets/tgs/SmokeBomb.json");
		if (bomb)
		{
			game->AddObject(bomb);
			bomb->mTransform->Position(gameObject->mTransform->Position());
		}
		isJump = false;
		StateChange(State::MOVE);
		mTrickRotate = XMVectorSet(0, 0, 0, 1);
	}
	mVelocity.y -= 9.81f * 3 * game->DeltaTime()->GetDeltaTime();
	mCC->Move(mVelocity * game->DeltaTime()->GetDeltaTime());

	if (mTrickPoint > mTrickCondition && !isTornado && IsUnrivaled())
	{
		AnimationChange(1);
		isTornado = true;
		CreateTornado();
		StateChange(State::TORNADO);
	}

}

void CCBoard::Tornado(float deltaTime)
{
	MoveSmokeParameterSet(0, mMaxSpeed);
	auto tornado = game->FindActor("Tornado");
	if (tornado)
	{
		mCurrentSpeed = 0.0f;
		mVelocity = XMVectorSet(0, 0, 0, 0);
		gameObject->mTransform->Position(tornado->mTransform->Position() + XMVectorSet(0, 5, 0, 0));
	}
	else
	{
		StateChange(State::JUMP);
	}
}

void CCBoard::Dead(float deltaTime)
{
	auto upRotate = XMQuaternionRotationAxis(gameObject->mTransform->Up(), 0.1f);
	auto leftRotate = XMQuaternionRotationAxis(gameObject->mTransform->Left(), 0.1f);
	auto mix = XMQuaternionMultiply(XMQuaternionMultiply(gameObject->mTransform->Quaternion(), upRotate), leftRotate);
	gameObject->mTransform->Quaternion(mix);

	auto mCC = gameObject->GetComponent<CharacterControllerComponent>();
	if (!mCC) return;
	mCurrentSpeed = 0.0f;
	mVelocity.x = 0;
	mVelocity.z = 0;
	mVelocity.y -= 9.81f * 3 * game->DeltaTime()->GetDeltaTime();
	mCC->Move(mVelocity * game->DeltaTime()->GetDeltaTime());
}

void CCBoard::StateChange(State next)
{
	mState = next;
}

//地面と平行にする処理
void CCBoard::GroundCheck()
{
	XMVECTOR pos = gameObject->mTransform->WorldPosition();
	auto mCC = gameObject->GetComponent<CharacterControllerComponent>();
	if (!mCC)return;

	XMVECTOR normal = XMVectorSet(0, 1, 0, 1);
	{
		//auto pos = gameObject->mTransform->WorldPosition();
		auto d = XMVectorSet(0, -1, 0, 1);
		RaycastHit hit;
		if (game->PhysX()->RaycastHit(pos, d, 100.0f, &hit)) {
			normal = hit.normal;
			normal = XMVector3Normalize(normal);
		}
	}

	{
		auto up = gameObject->mTransform->Up();
		up = XMVector3Normalize(up);
		auto dot = XMVector3Dot(normal, up).x;
		float angle = acos(dot);
		auto n = XMVector3Cross(up, normal);
		n = XMVector3Normalize(n);
		n.w = 1.0f;
		//angle = std::min(angle, (XM_PI * game->DeltaTime()->GetDeltaTime()));
		auto q = XMQuaternionRotationNormal(n, angle);
		auto wq = gameObject->mTransform->WorldQuaternion();
		q = XMQuaternionMultiply(wq, q);
		q = XMQuaternionNormalize(q);
		if (XMQuaternionIsInfinite(q) || XMQuaternionIsNaN(q)) {
			return;
		}
		gameObject->mTransform->WorldQuaternion(q);
	}
}

void CCBoard::Trick(float deltaTime)
{
	if (Input::Down(KeyCoord::Key_W)) {
		mTrickRotate.x -= mTrickRotatePower;
	}
	if (Input::Down(KeyCoord::Key_S)) {
		mTrickRotate.x += mTrickRotatePower;
	}
	if (Input::Down(KeyCoord::Key_A)) {
		mTrickRotate.y -= mTrickRotatePower;
	}
	if (Input::Down(KeyCoord::Key_D)) {

		mTrickRotate.y += mTrickRotatePower;
	}

	mTrickRotate.x -= Input::Analog(PAD_DS4_Velo3Coord::Velo3_Angular).y * 0.1f;
	mTrickRotate.y += Input::Analog(PAD_DS4_Velo3Coord::Velo3_Angular).x * 0.1f;

	mTrickRotate *= (deltaTime * 60);

	mTrickPoint = abs(mTrickRotate.x) + abs(mTrickRotate.y);

	auto xrotate = XMQuaternionRotationAxis(gameObject->mTransform->Left(), mTrickRotate.x);
	auto yrotate = XMQuaternionRotationAxis(gameObject->mTransform->Up(), mTrickRotate.y);
	/*auto xrotate = XMQuaternionRotationAxis(XMVectorSet(1,0,0,1), mTrickRotate.x);
	auto yrotate = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), mTrickRotate.y);*/
	auto mix = XMQuaternionMultiply(xrotate, yrotate);
	gameObject->mTransform->Quaternion(XMQuaternionMultiply(mix, gameObject->mTransform->Quaternion()));
}

void CCBoard::CreateTornado()
{
	if (game->FindActor("Tornado")) return;
	auto tornado = game->CreateActor("Assets/Effect/Tornado.json");
	if (tornado)
	{
		game->AddObject(tornado);
		PlaySE("Assets/PlayerSE/wind.wav");
		tornado->mTransform->Position(gameObject->mTransform->Position());
		auto parent = game->FindActor("Tornados");
		tornado->mTransform->SetParent(parent);
	}
}

void CCBoard::CreateAttackWind()
{
	if (IsUnrivaled())
	{
		auto temp = game->CreateActor("Assets/Effect/SpeedWind.json");
		if (temp)
		{
			if (!mSpeedEffect)
			{
				game->AddObject(temp);
				temp->mTransform->SetParent(gameObject);
				mSpeedEffect = temp;
			}
		}
	}
	else
	{
		if (mSpeedEffect)
		{
			game->DestroyObject(mSpeedEffect);
			mSpeedEffect = nullptr;
		}
	}
}

void CCBoard::MoveSmokeParameterSet(float speed, float max)
{
	auto smoke = game->FindActor("PlayerMoveSmoke");
	if (smoke)
	{
		auto smokes = smoke->mTransform->Children();
		for (auto&& s : smokes)
		{
			s->GetScript<MoveSmoke>()->SetMaxSpeed(max);
			s->GetScript<MoveSmoke>()->SetSpeed(speed);
		}
	}
}

bool CCBoard::Shake()
{
	auto f = abs(mPrevAcceler - Input::Analog(PAD_DS4_Velo3Coord::Velo3_Acceleration).z);
	if (f < -0.5f)
	{
		mPrevAcceler = Input::Analog(PAD_DS4_Velo3Coord::Velo3_Acceleration).z;
		return true;
	}
	mPrevAcceler = Input::Analog(PAD_DS4_Velo3Coord::Velo3_Acceleration).z;
	return false;
}

void CCBoard::PlaySE(std::string filename)
{
	auto component = gameObject->GetComponent<SoundComponent>();
	if (!component) return;
	component->SetLoop(false);
	component->LoadFile(filename);
	component->Play();
}

void CCBoard::AnimationChange(int id)
{
	//よくわからないバグが出てる
	auto bird = game->FindActor("Bird");
	if (!bird) return;
	auto mBirdAnimation = bird->GetComponent<AnimationComponent>();
	if (!mBirdAnimation) return;

	if (mBirdAnimation->mCurrentSet == id) return;
	auto anima1 = mBirdAnimation->GetAnimetionParam(mBirdAnimation->mCurrentSet);
	auto anima2 = mBirdAnimation->GetAnimetionParam(id);
	anima1.mWeight = 0;
	anima2.mWeight = 1;
	anima1.mTimeScale = 1;
	anima2.mTimeScale = 1;
	anima1.mTime = 0;
	anima2.mTime = 0;
	anima2.mLoop = false;
	mBirdAnimation->SetAnimetionParam(mBirdAnimation->mCurrentSet, anima1);
	mBirdAnimation->SetAnimetionParam(id, anima2);
	mBirdAnimation->mCurrentSet = id;
}

void CCBoard::SailRotateAnimation()
{
	auto sail = game->FindActor("Sail")->GetScript<CCSail>();
	if (!sail) return;

	auto bird = game->FindActor("Bird");
	if (!bird) return;
	auto mBirdAnimation = bird->GetComponent<AnimationComponent>();
	if (!mBirdAnimation) return;


	{
		if ((sail->GetSailRotateRad() <= 0 && mBirdAnimation->mCurrentSet == 3))
		{
			auto anima1 = mBirdAnimation->GetAnimetionParam(mBirdAnimation->mCurrentSet);
			anima1.mWeight = 0;

			auto anima2 = mBirdAnimation->GetAnimetionParam(0);
			anima2.mWeight = 1;
			anima2.mTimeScale = -2.5f;
			anima2.mTime = 61;
			anima2.mLoop = false;

			mBirdAnimation->SetAnimetionParam(mBirdAnimation->mCurrentSet, anima1);
			mBirdAnimation->SetAnimetionParam(0, anima2);
			mBirdAnimation->mCurrentSet = 0;
		}
		else if((sail->GetSailRotateRad() > 0 && mBirdAnimation->mCurrentSet == 4))
		{
			auto anima1 = mBirdAnimation->GetAnimetionParam(mBirdAnimation->mCurrentSet);
			anima1.mWeight = 0;

			auto anima2 = mBirdAnimation->GetAnimetionParam(0);
			anima2.mWeight = 1;
			anima2.mTimeScale = 2.5;
			anima2.mTime = 0;
			anima2.mLoop = false;

			mBirdAnimation->SetAnimetionParam(mBirdAnimation->mCurrentSet, anima1);
			mBirdAnimation->SetAnimetionParam(0, anima2);
			mBirdAnimation->mCurrentSet = 0;
		}
	}

	if (mBirdAnimation->mCurrentSet == 0)
	{
		auto anima = mBirdAnimation->GetAnimetionParam(0);
		if ((anima.mTime != 0 && anima.mTimeScale <= -1) || (anima.mTime != 61 && anima.mTimeScale >= 1)) return;
	}

	if (sail->GetSailRotateRad() <= 0)
	{
		auto anima1 = mBirdAnimation->GetAnimetionParam(mBirdAnimation->mCurrentSet);
		anima1.mWeight = 0;

		auto anima2 = mBirdAnimation->GetAnimetionParam(4);
		anima2.mWeight = 1;
		anima2.mTimeScale = 0;
		anima2.mTime = abs(sail->GetSailRotateRad()) * 10;
		anima2.mLoop = false;

		mBirdAnimation->SetAnimetionParam(mBirdAnimation->mCurrentSet, anima1);
		mBirdAnimation->SetAnimetionParam(4, anima2);
		mBirdAnimation->mCurrentSet = 4;
	}
	else
	{
		auto anima1 = mBirdAnimation->GetAnimetionParam(mBirdAnimation->mCurrentSet);
		anima1.mWeight = 0;

		auto anima2 = mBirdAnimation->GetAnimetionParam(3);
		anima2.mWeight = 1;
		anima2.mTimeScale = 0;
		anima2.mTime = abs(sail->GetSailRotateRad()) * 10;
		anima2.mLoop = false;

		mBirdAnimation->SetAnimetionParam(mBirdAnimation->mCurrentSet, anima1);
		mBirdAnimation->SetAnimetionParam(3, anima2);
		mBirdAnimation->mCurrentSet = 3;
	}
}
