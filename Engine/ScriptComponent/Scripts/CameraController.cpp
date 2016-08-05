#include "CameraController.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"SailBoard.h"
#include"CCBoard.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void CameraController::Initialize() {
	mIsDead = false;
	mTimer = 0;
	mAngle = 0.0f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void CameraController::Start() {
	mTarget = game->FindActor("Board");
	gameObject->mTransform->Position(XMVectorSet(0, 0, 0, 1));
}

//毎フレーム呼ばれます
void CameraController::Update()
{
	mTarget = game->FindActor("Board");
	if (!mTarget) return;
	if (!mTarget->GetScript<CCBoard>()) return;

	StateUpdate(game->DeltaTime()->GetDeltaTime());
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void CameraController::Finish() {

}

//コライダーとのヒット時に呼ばれます
void CameraController::OnCollideBegin(Actor* target) {
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void CameraController::OnCollideEnter(Actor* target) {
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void CameraController::OnCollideExit(Actor* target) {
	(void)target;
}

void CameraController::StateUpdate(float deltaTime)
{
	switch (mTarget->GetScript<CCBoard>()->GetState())
	{
	case State::STANDBY: Standby(deltaTime); break;
	case State::MOVE: Normal(deltaTime); break;
	case State::JUMP: Jump(deltaTime); break;
	case State::TORNADO: Tornado(deltaTime); break;
	case State::DEAD: Dead(deltaTime); break;
	}
}

void CameraController::Standby(float deltaTIme)
{
	mAngle += deltaTIme * 2;
	XMVECTOR nextPos;
	nextPos.x = (mTarget->mTransform->Position().x) + cos(mAngle) * 30;
	nextPos.y = mTarget->mTransform->Position().y + 10;
	nextPos.z = (mTarget->mTransform->Position().z) + sin(mAngle) * 30;

	mPosition = XMVectorLerp(mPosition, nextPos, 0.2f);

	auto at = XMMatrixLookAtLH(mPosition, mTarget->mTransform->Position(), gameObject->mTransform->Up());
	at = XMMatrixTranspose(at);
	gameObject->mTransform->Quaternion(XMQuaternionRotationMatrix(at));

	gameObject->mTransform->Position(mPosition);
}

void CameraController::Normal(float deltaTime)
{
	mPosition = XMVectorLerp(mPosition, mTarget->mTransform->Position() + (mTarget->mTransform->Forward() * -13) + XMVectorSet(0, 3.5f, 0, 1), 0.2f);

	auto at = XMMatrixLookAtLH(mPosition, mTarget->mTransform->Position(), gameObject->mTransform->Up());
	at = XMMatrixTranspose(at);
	gameObject->mTransform->Quaternion(XMQuaternionRotationMatrix(at));

	gameObject->mTransform->Position(mPosition);
}

void CameraController::Jump(float deltaTime)
{
	mPosition = XMVectorLerp(mPosition, mTarget->mTransform->Position() + XMVectorSet(0, 1, -5, 1), 0.2f);

	auto at = XMMatrixLookAtLH(mPosition, mTarget->mTransform->Position(), gameObject->mTransform->Up());
	at = XMMatrixTranspose(at);
	gameObject->mTransform->Quaternion(XMQuaternionRotationMatrix(at));

	gameObject->mTransform->Position(mPosition);
}

void CameraController::Dead(float deltaTime)
{
	if (!mIsDead)
	{
		mNextPos = mTarget->mTransform->Position() + XMVectorSet(0, -10, -5, 1);
		mIsDead = true;
	}
	mPosition = XMVectorLerp(mPosition, mNextPos, 0.2f);

	auto at = XMMatrixLookAtLH(mPosition, mTarget->mTransform->Position(), gameObject->mTransform->Up());
	at = XMMatrixTranspose(at);
	gameObject->mTransform->Quaternion(XMQuaternionRotationMatrix(at));

	gameObject->mTransform->Position(mPosition);
}

void CameraController::Tornado(float deltaTime)
{
	mPosition = XMVectorLerp(mPosition, mTarget->mTransform->Position() + XMVectorSet(0, 1, -50, 1), 0.2f);

	auto at = XMMatrixLookAtLH(mPosition, mTarget->mTransform->Position(), gameObject->mTransform->Up());
	at = XMMatrixTranspose(at);
	gameObject->mTransform->Quaternion(XMQuaternionRotationMatrix(at));

	gameObject->mTransform->Position(mPosition);
}


//プレイヤーを見る
void CameraController::Look()
{
	auto SubVector = gameObject->mTransform->Position() - mTarget->mTransform->Position();

	auto Angel = atan2(-SubVector.x, -SubVector.z);
	auto vec = XMVectorSet(0, 0, 0, 1);
	if (mTarget->Name() == "Tornado")
	{
		auto Angel2 = mTarget->mTransform->Rotate().x;
		vec = XMVectorSet(Angel2, Angel, 0, 1);
	}
	else
	{
		if (!mTarget->GetScript<SailBoard>()->GetIsJump())
		{
			auto Angel2 = mTarget->mTransform->Rotate().x;
			vec = XMVectorSet(Angel2, Angel, 0, 1);
		}
		else
		{
			vec = XMVectorSet(0, Angel, 0, 1);
		}
	}
	gameObject->mTransform->Rotate(vec);

}

XMVECTOR CameraController::Lerp(XMVECTOR p1, XMVECTOR p2)
{
	mTimer += 0.2f;
	mTimer = min(1, mTimer);
	return XMVectorLerp(p1, p2, mTimer);
}

