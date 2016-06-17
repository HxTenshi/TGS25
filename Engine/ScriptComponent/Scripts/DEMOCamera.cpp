#include "DEMOCamera.h"
#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Input/Input.h"
#include "Game/Component/TransformComponent.h"
#include "Engine\DebugEngine.h"
#include<math.h>
#include"SailBoard.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void DEMOCamera::Initialize(){
	mPrevJump = false;
	mTimer = 0;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void DEMOCamera::Start(){
	mTarget = game->FindActor("Board");
	gameObject->mTransform->Position(XMVectorSet(0, 0, 0, 1));
	offset = XMVectorSet(offsetx, offsety, offsetz,0);
	rotate = XMVectorSet(rotatex, rotatey, rotatez,0);
}

//毎フレーム呼ばれます
void DEMOCamera::Update(){

	if (mTarget)
	{
		

		auto temp = game->FindActor("Tornado");
		if (temp)
		{
			mTarget = temp;
			mPosition = mTarget->mTransform->Position() + offset;
			gameObject->mTransform->Rotate(rotate);
		}
		else
		{
			Look();

			//プレイヤーがジャンプしたらカメラの位置変更の準備
			if (mPrevJump != mTarget->GetScript<SailBoard>()->GetIsJump())
			{
				mFromPos = mPosition;
				mTimer = 0;
				mPrevJump = mTarget->GetScript<SailBoard>()->GetIsJump();
			}

			if (!mTarget->GetScript<SailBoard>()->GetIsJump())
			{
				mPosition = Lerp(mFromPos, mTarget->mTransform->Position() + (mTarget->mTransform->Forward() * -13) + XMVectorSet(0, 3.5f, 0, 1));
				if (Input::Down(PAD_DS4_KeyCoord::Button_DOWN))
				{
					mPosition = Lerp(mFromPos, mTarget->mTransform->Position() + (mTarget->mTransform->Forward() * 3) + XMVectorSet(0, 1, 0, 1));
				}
			}
			else
			{
				mPosition = Lerp(mFromPos, mTarget->mTransform->Position() + XMVectorSet(0, 1, -5, 1));
			}
			gameObject->mTransform->Position(mPosition);
		}
		gameObject->mTransform->Position(mPosition);
	}

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void DEMOCamera::Finish(){

}

//コライダーとのヒット時に呼ばれます
void DEMOCamera::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void DEMOCamera::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void DEMOCamera::OnCollideExit(Actor* target){
	(void)target;
}

void DEMOCamera::Look()
{
	auto SubVector = gameObject->mTransform->Position() - mTarget->mTransform->Position();

	auto Angel = atan2(-SubVector.x, -SubVector.z);
	auto vec = XMVectorSet(0, 0, 0, 1);
	if (!mTarget->GetScript<SailBoard>()->GetIsJump())
	{
		auto Angel2 = mTarget->mTransform->Rotate().x;
		vec = XMVectorSet(Angel2, Angel, 0, 1);
	}
	else
	{
		vec = XMVectorSet(0, Angel, 0, 1);
	}
	gameObject->mTransform->Rotate(vec);
}

XMVECTOR DEMOCamera::Lerp(XMVECTOR p1, XMVECTOR p2)
{
	mTimer += 0.2f;
	mTimer = min(1, mTimer);
	return XMVectorLerp(p1, p2, mTimer);
}
