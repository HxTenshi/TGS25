#include "CCSail.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void CCSail::Initialize() 
{
	mBirdPos = BirdPosition::RIGHT;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void CCSail::Start()
{
	Speed = 1.0f;
	RotatePower = 0.25f;
}

//毎フレーム呼ばれます
void CCSail::Update()
{
	if (isAnimation)
	{
		SailAnimation();
	}
	else
	{
		SailRotate();
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void CCSail::Finish() {

}

//コライダーとのヒット時に呼ばれます
void CCSail::OnCollideBegin(Actor* target) {
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void CCSail::OnCollideEnter(Actor* target) {
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void CCSail::OnCollideExit(Actor* target) {
	(void)target;
}

BirdPosition CCSail::GetBirdPosition()
{
	return mBirdPos;
}

//返り値は
float CCSail::MovePower()
{
	//auto wind = gameObject->mTransform->GetParent()->mTransform->GetParent()->GetScript<SailBoard>();
	//if (!wind)return 0.0f;
	//auto windvec = wind->GetWind();
	auto windvec = XMVectorSet(1, 0, 0, 0);
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

float CCSail::GetSailRotateRad()
{
	return mSailRotate;
}

float CCSail::Lerp(float value1, float value2, float amount)
{
	return (value1 * (1.0f - amount)) + (value2 * amount);
}

void CCSail::SailRotate()
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

void CCSail::SailAnimation()
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
