#include "Enemy.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Engine\DebugEngine.h"

Enemy::~Enemy() {
	gameObject->mTransform->AllChildrenDestroy();
}

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Initialize(){
	mSpeed = 0.01f;
	mSize = gameObject->mTransform->Scale().z;
	mPositionY = 0.0f;
	mRotateY = 0.0f;
	mDeadHeigth = -10.0f;
	mIsHit = false;
	mIsSearchRange = false;

	//game->CreateActor("");
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Start(){
	//game->Debug()->Log(std::to_string(mSize));

	auto nowPosition = gameObject->mTransform->Position();
	mPositionY = nowPosition.y;
	gameObject->mTransform->Position(nowPosition);
}

//���t���[���Ă΂�܂�
void Enemy::Update(){

	/*PlayerSearch(gameObject);

	if (mIsSearchRange) {
		PlayerChase(gameObject);
	}
	else {
		SearchMove();
	}*/
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Enemy::OnCollideBegin(Actor* target){
	(void)target;
	if (target->Name() == "Board"){
		mIsHit = true;
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Enemy::OnCollideEnter(Actor* target){
	(void)target;
	if (target->Name() == "Board") {
		mIsHit = true;
	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Enemy::OnCollideExit(Actor* target){
	(void)target;
	mIsHit = false;
}

//float Enemy::norm(XMVECTOR a) {
//	return a.x * a.x + a.z * a.z;
//}
//
//float Enemy::dot(XMVECTOR a, XMVECTOR b) {
//	return a.x * b.x + a.z * b.z;
//}
//
//float Enemy::cross(XMVECTOR a, XMVECTOR b) {
//	return a.x * b.z - a.z * b.x;
//}
//
//int Enemy::CCW(XMVECTOR p0, XMVECTOR p1, XMVECTOR p2) {
//	auto a = p1 - p0;
//	auto b = p2 - p0;
//	if(cross(a, b) > )
//	return 0;
//}
//
//bool Enemy::IsInSector(XMVECTOR c, float a, float b, float r, XMVECTOR p) {
//	float distX = abs(c.x - p.x);
//	float distZ = abs(c.z - p.z);
//	if (distX > a && distZ > a) return false;
//
//	auto v1 = XMVectorSet(
//		a * cosf((r + b / 2) * 3.14 / 180),
//		gameObject->mTransform->Position().y,
//		a * sinf((r + b / 2) * 3.14 / 180),
//		0.0f);
//	auto v2 = XMVectorSet(
//		a * cosf((r - b / 2) * 3.14 / 180),
//		gameObject->mTransform->Position().y,
//		a * sinf((r - b / 2) * 3.14 / 180),
//		0.0f);
//
//	return false;
//}

void Enemy::PlayerColorChange(Actor* obj) {
	// �F�̐ݒ�
	auto color = XMFLOAT4(1, 1, 1, 1);
	// ��苗�������ƐF�̒l��ς���
	if (mIsSearchRange) {
		color = XMFLOAT4(1, 0, 0, 1);
	}
	else {
		color = XMFLOAT4(1, 1, 1, 1);
	}
	// �F�̍X�V
	auto mate = gameObject->GetComponent<MaterialComponent>();
	if (mate) {
		game->Debug()->Log(std::to_string(color.y));
		mate->SetAlbedoColor(color);
	}
}

void Enemy::SearchMove() {
	auto v = gameObject->mTransform->Forward() * mSpeed;
	auto position = gameObject->mTransform->Position();
	gameObject->mTransform->Position(position + v);

	mRotateY += 3.14 / 180;
	
	auto rotate = XMVectorSet(0.0f, mRotateY, 0.0f, 0.0f);

	gameObject->mTransform->Rotate(rotate);
}

void Enemy::PlayerSearchMode(Actor* thisObj) {
	// �v���C���[�̑{��
	auto object = game->FindActor("Board");
	if (!object) return;

	// �v���C���[�Ƃ̋����̌v�Z
	auto target = ((object->mTransform->Position()) - thisObj->mTransform->Position());
	auto targetRange = XMVector3Length(object->mTransform->Position() - thisObj->mTransform->Position());
	// ���K��
	target = XMVector3Normalize(target);

	auto thisObjPosition = thisObj->mTransform->Position();
	auto objPosition = object->mTransform->Position();

	auto startAngle = XMVectorSet(thisObjPosition.x + sinf(0.0f), 0.0f, thisObjPosition.z + cosf(0.0f), 0.0f);
	auto endAngle = XMVectorSet(objPosition.x + sinf(70.0f), 0.0f, objPosition.z + cosf(70.0f), 0.0f);

	if (mIsHit == false) {
		// ��苗���Ȃ甽������ �͈͊O�Ȃ�
		if (targetRange.x <= (mSize / 2) + (mSize * 3)) {
			// �v���C���[�Ƃ̃��W�A���̌v�Z
			auto radius = XMVector2AngleBetweenVectors(thisObjPosition, objPosition);
			// ���W�A����x���ɕϊ�
			auto angle = XMConvertToDegrees(radius.x);

			//return true;
			mIsSearchRange = true;

			//if (startAngle.x <= objPosition.x <= endAngle.x) {
			//	if (startAngle.z >= objPosition.z >= endAngle.z) {
			//		mIsSearchRange = true;
			//	}
		}
		else if (targetRange.x >= (mSize / 2) + (mSize * 5)) {
			//return false;
			mIsSearchRange = false;
		}
	}

	//return false;
}

void  Enemy::PlayerChaseMode(Actor* thisObj) {
	// �v���C���[�̑{��
	auto object = game->FindActor("Board");
	if (!object) return;

	// �v���C���[�Ƃ̋����̌v�Z
	auto target = ((object->mTransform->Position()) - thisObj->mTransform->Position());
	auto targetRange = XMVector3Length(object->mTransform->Position() - thisObj->mTransform->Position());
	// ���K��
	target = XMVector3Normalize(target);

	// �ړ�
	if (mIsHit == false) {
		if (mIsSearchRange) {
			auto moveSpeed = thisObj->mTransform->Forward() * -mSpeed;
			auto nowPosition = thisObj->mTransform->Position();
			// �v���C���[�̕���������
			auto mRotate = XMVectorSet(target.x, 0.0f, target.z, target.w);
			//thisObj->mTransform->Quaternion(target);
			gameObject->mTransform->Quaternion(mRotate);
		}
	}
}

void Enemy::PlayerChase(Actor* thisObj) {
	// �ړ�
	if (mIsHit == false) {
		auto moveSpeed = gameObject->mTransform->Forward() * -mSpeed;
		auto movePosition = gameObject->mTransform->Position();

		gameObject->mTransform->Position(movePosition + moveSpeed);
	}
}

void Enemy::PlayerSpaceChase(Actor* thisObj, const int interval, int count) {
	// �v���C���[�̑{��
	auto object = game->FindActor("Board");
	if (!object) return;

	auto target = ((object->mTransform->Position()) - thisObj->mTransform->Position());
	auto targetRange = XMVector3Length(object->mTransform->Position() - thisObj->mTransform->Position());

	target = XMVector3Normalize(target);

	//target.y = 0;

	// �ړ�
	if (mIsHit == true) {
		gameObject->mTransform->Forward() * 0;
	}
	else {
		// ��苗���Ȃ甽������
		if (targetRange.x <= (mSize / 2) + (mSize * 6)) {
			if (count % interval < interval / 2) {
				gameObject->mTransform->AddForce(target*mSpeed);
			}
			mIsSearchRange = true;
		}
		else if (targetRange.x >(mSize / 2) + (mSize * 6)) {
			gameObject->mTransform->Forward() * 0;
			mIsSearchRange = false;
		}
	}
}

// ��苗���܂ŗ������玀�S
void Enemy::FallDead(Actor* thisObj) {
	// ���̈ʒu�܂ŗ������玀�S
	if (thisObj->mTransform->Position().y <= mDeadHeigth) {
		game->DestroyObject(thisObj);
	}
}