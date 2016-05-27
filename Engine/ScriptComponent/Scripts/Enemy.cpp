#include "Enemy.h"
#include "PlayerSearch.h"
#include "SailBoard.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Engine\DebugEngine.h"

Enemy::~Enemy() {
	//gameObject->mTransform->AllChildrenDestroy();
}

//生成時に呼ばれます（エディター中も呼ばれます）
void Enemy::Initialize(){
	mEnemyState = EnemyState::PlayerSearch;
	mAddSearchObjCount = 0;
	mDamage = 0;
	mSpeed = 1.0f * 0.01f;
	mSize = gameObject->mTransform->Scale();
	mHalfSizeZ = mSize.z / 2.0f;
	mPositionY = gameObject->mTransform->Position().y;
	mRotateY = 0.0f;
	mScalarX = 1.0f;
	mScalarY = 2.0f;
	mScalarZ = 3.0f;
	//mDeadHeigth = -10.0f;
	mIsFloorHit = false;
	mIsImmortalBody = false;
	mIsDistanceAct = false;
	mIsAttckMode = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Enemy::Start(){
	// 親の取得	
	mParentObj = gameObject->mTransform->GetParent();
}

//毎フレーム呼ばれます
void Enemy::Update(){
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Enemy::Finish(){
	/*game->DestroyObject(mPlayerSearchObj);*/
}

//コライダーとのヒット時に呼ばれます
void Enemy::OnCollideBegin(Actor* target){
	if (target->Name() == "Board"){
		auto playerScript = target->GetComponent<SailBoard>();
		// プレイヤーが無敵状態なら死亡
		// そうでない場合はプレイヤーにダメージを与える
		if (playerScript->IsUnrivaled()) {
			game->DestroyObject(gameObject);
		}
		else {
			playerScript->Damage(mDamage);
		}
	}
}

//コライダーとのヒット中に呼ばれます
void Enemy::OnCollideEnter(Actor* target){
	if (target->Name() == "Floor") {
		mIsFloorHit = true;
	}
}

//コライダーとのロスト時に呼ばれます
void Enemy::OnCollideExit(Actor* target){
	if (target->Name() == "Floor") {
		mIsFloorHit = false;
	}
}
// 敵の色の設定です
void Enemy::PlayerColorChange() {
	// 色の設定
	auto color = XMFLOAT4(1, 1, 1, 1);
	// 一定距離内だと色の値を変える
	if (mEnemyState == EnemyState::PlayerChase) {
		color = XMFLOAT4(1, 0, 0, 1);
	}
	else {
		color = XMFLOAT4(1, 1, 1, 1);
	}
	// 色の更新
	auto mate = gameObject->GetComponent<MaterialComponent>();
	if (mate) {
		//game->Debug()->Log(std::to_string(color.y));
		mate->SetAlbedoColor(color);
	}
}
// 索敵移動をします（デフォルト設定）
void Enemy::SearchMove() {
	// デフォルトの移動
	auto position = gameObject->mTransform->Position();
	auto forward = gameObject->mTransform->Forward();
	gameObject->mTransform->Position(position + forward);
}
// 索敵関数です
void Enemy::PlayerSearchMode(const XMVECTOR objScale) {
	// 親の取得
	mParentObj = gameObject->mTransform->GetParent();

	// 索敵オブジェの作成
	if (mAddSearchObjCount < 1) {
		mPlayerSearchObj = game->CreateActor("Assets/PlayerSearchRange");
		game->AddObject(mPlayerSearchObj);
		//game->Debug()->Log("親検索");

		// 索敵オブジェのスクリプト取得
		searchScript = mPlayerSearchObj->GetScript<PlayerSearch>();
		// 大きさの変更
		auto setScale = XMVectorSet(
			objScale.x * mScalarX, objScale.y * mScalarY, objScale.z * mScalarZ, 0.0f);
		mPlayerSearchObj->mTransform->Scale(setScale);
		searchScript->SetScalarZ(mScalarZ);

		// 索敵範囲オブジェの位置設定
		auto childrenScale = mPlayerSearchObj->mTransform->Scale();
		// 位置の指定
		auto setPosition = XMVectorSet(0.0f, 0.0f, (objScale.z + childrenScale.z) / 2.0f, 0.0f);
		mSearchObjPosition = setPosition;

		// 位置の変更
		mPlayerSearchObj->mTransform->Position(-setPosition);
		game->Debug()->Log(std::to_string(mPlayerSearchObj->mTransform->Position().z));

		// 敵の子供に追加する
		mPlayerSearchObj->mTransform->SetParent(mParentObj);

		mAddSearchObjCount = 1;
	}

	// 索敵オブジェの位置更新
	mPlayerSearchObj->mTransform->Position(-mSearchObjPosition);

	if (searchScript->IsPlayerSearch()|| mIsAttckMode){
		if (!mIsDistanceAct) {
			mEnemyState = EnemyState::PlayerChase;
		}
		else {
			auto searchObjScale = mPlayerSearchObj->mTransform->Scale().z;
			/*int PlayerDistanceNumber = 1 + (searchScript->PlayerDistance() / (searchObjScale / 3.0f));
			if (PlayerDistanceNumber == 4) {
				PlayerDistanceNumber = 3;
			}*/
			// 配列にしてif文を無くす
			if (searchScript->PlayerDistance() <= searchObjScale / 3.0f) {
				mEnemyState = EnemyState::PlayerShortDistance;
			}
			else if (searchScript->PlayerDistance() <= searchObjScale / 2.0f) {
				mEnemyState = EnemyState::PlayerCenterDistance;
			}
			else if (searchScript->PlayerDistance() > searchObjScale / 2.0f) {
				mEnemyState = EnemyState::PlayerLongDistance;
			}
		}
	}
	else {
		mEnemyState = EnemyState::PlayerSearch;
	}
}

// プレイヤーの方向に回転します(未完成)
void  Enemy::PlayerChaseMode() {
	// プレイヤーの捜索
	// プレイヤーの方向を向く（まだ）
	auto playerObj = game->FindActor("Board");
	auto playerPosition = playerObj->mTransform->Position();
	// 親のステータスの取得
	auto parentPosition = mParentObj->mTransform->Position();
	
	auto v = parentPosition - playerPosition;
	auto angle = atan2(v.x, v.z);

	mParentObj->mTransform->Quaternion(XMQuaternionRotationAxis(mParentObj->mTransform->Up(), angle));
}
// プレイヤーを追跡します（デフォルト設定）
void Enemy::PlayerChase() {
	// 移動
	auto position = gameObject->mTransform->Position();
	auto forward = gameObject->mTransform->Forward();
	gameObject->mTransform->Position(position + forward);
}

// プレイヤーとの距離が短いときの行動です（デフォルト設定）
void Enemy::ShortDistanceAttack() {
	// 移動
	auto position = gameObject->mTransform->Position();
	auto forward = gameObject->mTransform->Forward();
	gameObject->mTransform->Position(position + forward);
}

// プレイヤーとの距離が中間の行動です（デフォルト設定）
void Enemy::CenterDistanceAttack() {
	// 移動
	auto position = gameObject->mTransform->Position();
	auto forward = gameObject->mTransform->Forward();
	gameObject->mTransform->Position(position + forward);
}

// プレイヤーとの距離が長いときの行動です（デフォルト設定）
void Enemy::LongDistanceAttack() {
	// 移動
	auto position = gameObject->mTransform->Position();
	auto forward = gameObject->mTransform->Forward();
	gameObject->mTransform->Position(position + forward);
}

// 敵の目の前にオブジェを生成します
void Enemy::SetForwardObj(Actor* setObj) {
	auto parentPosition = mParentObj->mTransform->Position();
	auto parentRotate = mParentObj->mTransform->Rotate();
	auto parentScale = gameObject->mTransform->Scale();
	/*auto objPosition = gameObject->mTransform->Position();
	auto objScale = gameObject->mTransform->Scale();*/
	auto setObjScale = setObj->mTransform->Scale();

	// 位置の指定
	auto setPosition = XMVectorSet(
		((parentScale.z / 2.0f) + (setObjScale.z / 2.0f)) * sinf(parentRotate.y),
		0.0f,
		((parentScale.z / 2.0f) + (setObjScale.z / 2.0f)) * cosf(parentRotate.y), 0.0f);
	// 位置の変更
	setObj->mTransform->Position(parentPosition + -setPosition);
	// 生成元のオブジェの回転角に変更
	setObj->mTransform->Rotate(parentRotate);
}

// ダメージの設定です
void Enemy::setDamage(int damage) {
	mDamage = damage;
}

// 一定距離まで落ちたら死亡します
void Enemy::FallDead(Actor* thisObj) {
	// 一定の位置まで落ちたら死亡
	if (thisObj->mTransform->Position().y <= mDeadHeigth) {
		game->DestroyObject(mPlayerSearchObj);
		game->DestroyObject(gameObject);
		game->DestroyObject(thisObj);
	}
}

// プレイヤーとの距離の計算をします (Z座標)
float Enemy::PlayerDistance() {
	// プレイヤーの検索
	auto object = game->FindActor("Board");
	//if (!object) return 0.0f;
	// プレイヤーとの距離の計算
	auto targetRange = XMVector3Length(
		object->mTransform->Position() - gameObject->mTransform->Position());
	return targetRange.z;
}
// 敵の行動関数
void Enemy::Move() {
	PlayerSearchMode(gameObject->mTransform->Scale());

	if (mEnemyState == EnemyState::PlayerSearch) {
		SearchMove();
	}

	if (!mIsDistanceAct) {
		if (mEnemyState == EnemyState::PlayerChase) {
			PlayerChase();
			//PlayerChaseMode();
		}
	}
	else {

		if (mEnemyState == EnemyState::PlayerShortDistance) {
			ShortDistanceAttack();
		}
		else if (mEnemyState == EnemyState::PlayerCenterDistance) {
			CenterDistanceAttack();
		}
		else if (mEnemyState == EnemyState::PlayerLongDistance) {
			LongDistanceAttack();
		}
	}

	Enemy::PlayerColorChange();
	Enemy::FallDead(mParentObj);
}