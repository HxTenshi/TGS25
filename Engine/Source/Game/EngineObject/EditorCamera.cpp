
#include "EditorCamera.h"

#include "MySTL/ptr.h"

#include "Window/Window.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/CameraComponent.h"

#include "Input/Input.h"

EditorCamera::EditorCamera(){
	mRClickMousePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//mRClickEyeVect = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//mRClickRotateAxis = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	mCameraComponent = new CameraComponent();
	mCamera.mTransform = make_shared<TransformComponent>();
	mCamera.AddComponent<TransformComponent>(mCamera.mTransform);
	mCamera.AddComponent(shared_ptr<CameraComponent>(mCameraComponent));

	//UINT width = WindowState::mWidth;
	//UINT height = WindowState::mHeight;
	//XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);
	mCamera.mTransform->Position(XMVectorSet(0, 3, -10, 1));
	mCamera.mTransform->Scale(XMVectorSet(1, 1, 1, 1));
}
EditorCamera::~EditorCamera(){
	mCamera.Finish();
}
void EditorCamera::Initialize(){
	mCamera.Finish();
	mCamera.Initialize();
	mCamera.Start();
}
void EditorCamera::Update(float deltaTime){
	auto pos = XMVectorSet(0, 0, 0, 1);
	if (Input::Down(KeyCoord::Key_A)){
		pos.x -= 0.1f;
	}
	if (Input::Down(KeyCoord::Key_D)){
		pos.x += 0.1f;
	}
	if (Input::Down(KeyCoord::Key_W)){
		pos.z += 0.1f;
	}
	if (Input::Down(KeyCoord::Key_S)){
		pos.z -= 0.1f;
	}
	if (Input::Down(KeyCoord::Key_Q)){
		pos.y += 0.1f;
	}
	if (Input::Down(KeyCoord::Key_E)){
		pos.y -= 0.1f;
	}
	if (Input::Trigger(MouseCoord::Right)){
		int x, y;
		Input::MousePosition(&x, &y);
		mRClickMousePos = XMVectorSet((FLOAT)x, (FLOAT)y, 0.0f, 0.0f);


		//mRClickEyeVect = mCamera.mTransform->Forward();
		//mRClickRotateAxis = XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), -XMVector3Normalize(mRClickEyeVect));

		//mRClickRotate = mCamera.mTransform->Rotate();

	}
	if (Input::Down(MouseCoord::Right)){
		int x, y;
		Input::MousePosition(&x, &y);
		XMVECTOR DragVect = XMVectorSet((FLOAT)x, (FLOAT)y, 0.0f, 0.0f) - mRClickMousePos;

		DragVect *= 0.01f;
		XMVECTOR xrot = XMQuaternionRotationAxis(mCamera.mTransform->Left(), DragVect.y);
		//Eye = XMVector3Rotate(-mRClickEyeVect, xrot) + At;

		XMVECTOR yrot = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), DragVect.x);
		//XMVECTOR yrot = XMQuaternionRotationAxis(mCamera.mTransform->Up(), DragVect.x);
		//Eye = XMVector3Rotate(-mRClickEyeVect, yrot) + At;

		XMVECTOR rot = XMQuaternionMultiply(xrot, yrot);
		XMVECTOR rotate = mCamera.mTransform->Rotate();
		rot = XMQuaternionMultiply(rotate, rot);
		mCamera.mTransform->Rotate(rot);
		mRClickMousePos = XMVectorSet((FLOAT)x, (FLOAT)y, 0.0f, 0.0f);

	}
	auto move = mCamera.mTransform->Position();
	move += mCamera.mTransform->Forward() * pos.z;
	move += mCamera.mTransform->Left() * pos.x;
	move.y += pos.y;

	mCamera.mTransform->Position(move);
	mCamera.UpdateComponent(deltaTime);
}

XMVECTOR EditorCamera::ScreenToWorldPoint(const XMVECTOR& point){
	XMVECTOR d;
	XMMATRIX projInv;
	projInv = XMMatrixInverse(&d, mCameraComponent->mProjection);
	XMMATRIX viewInv;
	viewInv = XMMatrixInverse(&d, mCameraComponent->mView);

	XMMATRIX vpInv;
	vpInv = XMMatrixIdentity();
	vpInv._11 = WindowState::mWidth / 2.0f; vpInv._22 = -(WindowState::mHeight / 2.0f);
	vpInv._41 = WindowState::mWidth / 2.0f; vpInv._42 = WindowState::mHeight / 2.0f;

	vpInv = XMMatrixInverse(&d, vpInv);

	//vpInv = XMMatrixMultiply(projInv, viewInv);
	//vpInv = XMMatrixMultiply(projInv, viewInv);
	vpInv = vpInv * projInv * viewInv;
	//vpInv = projInv * viewInv;

	//float nearFar = 100.0f - 0.01f;

	XMVECTOR worldPoint = point;// XMVectorSet(WindowState::mWidth, WindowState::mHeight, nearFar, 1.0f);
	worldPoint.z = 1.0f;
	worldPoint = XMVector3TransformCoord(worldPoint, vpInv);
	return worldPoint;
}

XMVECTOR EditorCamera::PointRayVector(const XMVECTOR& point){

	XMVECTOR end = ScreenToWorldPoint(point);
	XMVECTOR vect = end - mCamera.mTransform->Position();
	vect = XMVector3Normalize(vect);
	return vect;
}

XMVECTOR EditorCamera::GetPosition(){
	return mCamera.mTransform->Position();
}