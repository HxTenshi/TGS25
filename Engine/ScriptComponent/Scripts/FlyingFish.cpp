#include "FlyingFish.h"
#include "Enemy.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Engine\DebugEngine.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void FlyingFish::Initialize(){
	Enemy::Initialize();

	mUpCount = mUpInterval / 2;
	mInitUpCount = mUpInterval;
	mUpCosine = 0.0f;
	mWallHitCount = 0;
	mInitJampRestTime = mJampRestTime;
	mRotateInterval = 2.0f;
	mSpeed = mSetSpeed;
	mIsJamp = true;
	mIsInitSet = false;
	mIsFloorHit = false;
	mJampVelocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void FlyingFish::Start(){
	Enemy::Start();
	Enemy::SetDamage(mSetDamage);
	Enemy::SetResPawnTime(mSetResPawnTime);
	Enemy::AddPlayerChaseStopDistance(mAddChaseStopDistance);
	Enemy::SetSearchRangeScale(mSetSearchRengeScaleX, mSetSearchRengeScaleY, mSetSearchRengeScaleZ);

	auto floorObj = game->FindActor("Floor");
	mFloorPosition = floorObj->mTransform->Position().y;
}

//���t���[���Ă΂�܂�
void FlyingFish::Update(){

	Enemy::Move();

	/*if (!mIsInitSet) mInitParentPositionY = -1.5f;*/
	// mIsJamp��false�Ȃ�W�����v�����Ă��Ȃ�
	if (mIsJamp == false) {
		// mJampRestTime���O�ɂȂ�����ēx�W�����v����
		if (mJampRestTime <= 0) {
			mJampRestTime = mInitJampRestTime;
		}
		else if (mJampRestTime > 0) {
			mJampRestTime--;
		}

		mUpCount = mUpInterval / 2;
	}
	else {
		// �W�����v��
		// mUpCount���O�ɂȂ�Ɨ�����
		if (mUpCount > 0) {
			mUpCount--;
		}
		else {
			mUpCount = 0;
		}
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void FlyingFish::Finish(){
	Enemy::Finish();
	mUpCount = 0;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void FlyingFish::OnCollideBegin(Actor* target){
	Enemy::OnCollideBegin(target);
	if (target->Name() == "Floor") {
		// �ڐG�������̈ʒu������
		mFloorPosition = target->mTransform->Position().y;
		// ���ɐڐG������true
		if (!mIsInitSet && mParentObj->mTransform->Position().y <= mFloorPosition) {
			mIsInitSet = true;
		}
		// �����̍����ȉ��ƂȂ�����ēx�W�����v�t���O��false�ɂ���
		if (mParentObj->mTransform->Position().y <= mFloorPosition) {
			mIsJamp = false;
		}

		mIsFloorHit = true;
	}

	if (target->Name() == "Wall") {
		mWallHitCount++;
		mRotateY =  mRotateY + 3.14f - (3.14f / 180.0f / mRotateInterval);
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void FlyingFish::OnCollideEnter(Actor* target){
	Enemy::OnCollideEnter(target);

	if (target->Name() == "Floor") {
		if (!mIsInitSet && mParentObj->mTransform->Position().y <= mFloorPosition) {
			mIsInitSet = true;
		}

		if (mParentObj->mTransform->Position().y <= mFloorPosition) {
			mIsJamp = false;
			mUpCosine = 0.0f;
		}
	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void FlyingFish::OnCollideExit(Actor* target){
	if (target->Name() == "Floor") mIsFloorHit = false;
}

void FlyingFish::SearchMove() {
	auto parentRotate = mParentObj->mTransform->Rotate();
	// �G�̉�]
	if (mWallHitCount % 2 == 0) {
		//mRotateY += 3.14f / 180.0f / mRotateInterval;
		parentRotate.y += 3.14f / 180.0f / mRotateInterval;
	}
	else {
		//mRotateY -= 3.14f / 180.0f / mRotateInterval;
		parentRotate.y -= 3.14f / 180.0f / mRotateInterval;
	}

	if (mRotateY >= 3.14f * 2.0f) {
		//mRotateY = 0.0f;
		parentRotate.y = 0.0f;
	}
	mParentObj->mTransform->Rotate(parentRotate);
	
	JampMove();
}

void FlyingFish::PlayerChase() {
	Enemy::PlayerChaseMode();
	JampMove();
}

void FlyingFish::JampMove() {
	auto parentPosition = mParentObj->mTransform->Position();
	auto forwardMove = mParentObj->mTransform->Forward() * -mSpeed * 0.01f;
	// �W�����v�Ԋu
	if (mJampRestTime <= 0) {
		/*if (mUptekitou < 3.141593f / 2.0f) {
			mUptekitou += 3.141593f / mInitUpCount;
		}*/
		mUpCosine += 3.141593f / mInitUpCount;
		mAnimationID = 1;

		if (mUpCount > 0) {
			// �グ�鏈��
			/*mJampVelocity =
				mParentObj->mTransform->Up() * (mUpPowar + (mUpCount / mUpInterval));*/
			// �R�T�C���ɂ��W�����v
			mJampVelocity = mParentObj->mTransform->Up() * mUpPowar * cosf(mUpCosine) * 0.01f;
			//game->Debug()->Log(std::to_string(cosf(mUpCosine)));
			

			//mJampRestTime = mInitJampRestTime;
			mIsJamp = true;
		}
		else {
			if (parentPosition.y > mFloorPosition) {
				// �����鏈��
				auto downVelocity = XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);
				/*mJampVelocity = downVelocity * -((mUpPowar)+(mDownCount / mUpInterval));*/

				mJampVelocity = mParentObj->mTransform->Up() * mUpPowar * cosf(mUpCosine) * 0.01f;
				//game->Debug()->Log(std::to_string(cosf(mUpCosine)));

				// ���ƐڐG���Ă���ꍇ�͐�΂ɏ����l�ɖ߂�悤�ɕ␳
				// �ڐG���Ă��Ȃ������痎���Ă���
				/*if (mIsFloorHit) {*/
				if (parentPosition.y + mJampVelocity.y < mFloorPosition) {
					if (mIsFloorHit) {
						auto revasionVelocity = XMVectorSet(
							0.0f,
							(float)abs(parentPosition.y) - (float)abs(mFloorPosition),
							0.0f, 0.0f);
						mJampVelocity = revasionVelocity;
					}
					else {
						mJampVelocity = -mParentObj->mTransform->Up() * mUpPowar * 0.01f;
					}	
				}			
				
			}
		}
	}
	// �����i�K�ŏ��ƐڐG���Ă��Ȃ�������
	if (mIsInitSet) {
		// �ڐG������W�����v����
		if (mIsJamp) {
			mParentObj->mTransform->Position(parentPosition + forwardMove + mJampVelocity);
		}
		else {
			mParentObj->mTransform->Position(parentPosition + forwardMove);
			mAnimationID = 0;
		}
	}
	else {
		// ���܂ŗ�����
		mParentObj->mTransform->Position(parentPosition - mParentObj->mTransform->Up() * (10.0f * 0.01f));
	}
}