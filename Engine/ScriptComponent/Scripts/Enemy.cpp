#include "Enemy.h"
#include "PlayerSearch.h"
#include "SailBoard.h"
#include "ParentObj.h"

#include "Game/Actor.h"
#include "Game/Script/IGame.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Game/Component/PhysXColliderComponent.h"
#include "Engine\DebugEngine.h"

Enemy::~Enemy() {
	//gameObject->mTransform->AllChildrenDestroy();
	//game->DestroyObject(mEnemyCGObj);
}

//生成時に呼ばれます（エディター中も呼ばれます）
void Enemy::Initialize(){
	
	mEnemyState = EnemyState::PlayerSearch;
	mAddSearchObjCount = 0;
	mDamage = 0;
	mResPawnTime = 0;
	mCGCreateCount = 0;
	mSpeed = 1.0f * 0.01f;
	mSize = gameObject->mTransform->Scale();
	mHalfSizeZ = mSize.z / 2.0f;
	mPositionY = gameObject->mTransform->Position().y;
	mRotateY = 0.0f;
	mScalarX = 1.0f;
	mScalarY = 2.0f;
	mScalarZ = 3.0f;
	mIsFloorHit = false;
	mIsImmortalBody = false;
	mIsDistanceAct = false;
	mIsAttckMode = false;
	mIsDead = false;

	// 重複処理をしてしまってまずいことになる
	mDistanceVector.push_back(EnemyState::PlayerShortDistance);
	mDistanceVector.push_back(EnemyState::PlayerCenterDistance);
	mDistanceVector.push_back(EnemyState::PlayerLongDistance);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Enemy::Start(){
	// 親の取得	
	//mParentObj = gameObject->mTransform->GetParent();
	mParentObj = gameObject->mTransform->GetParent();
}

//毎フレーム呼ばれます
void Enemy::Update(){
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Enemy::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Enemy::OnCollideBegin(Actor* target){
	if (target->Name() == "Board"){
		auto playerScript = target->GetScript<SailBoard>();

		//// プレイヤーが無敵状態なら死亡
		//// そうでない場合はプレイヤーにダメージを与える
		//if (playerScript->IsUnrivaled() || playerScript->IsTrick()) {
		//	game->DestroyObject(gameObject);
		//}
		//else {
		//	playerScript->Damage(mDamage);
		//}

		// ダメージを与えて
		playerScript->Damage(mDamage);
		mIsDead = true;

		game->DestroyObject(mPlayerSearchObj);
		game->DestroyObject(gameObject);
		game->DestroyObject(gameObject->mTransform->GetParent());

		/*auto houkou = gameObject->mTransform->Position() - target->mTransform->Position();
		mKnockBackHoukou = XMVector3Normalize(houkou);*/
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
	if (mate) mate->SetAlbedoColor(color);
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
	//mParentObj = gameObject->mTransform->GetParent();

	// 索敵オブジェの作成
	if (mAddSearchObjCount < 1) {
		mPlayerSearchObj = game->CreateActor("Assets/Enemy/PlayerSearchRange");
		game->AddObject(mPlayerSearchObj);
		mPlayerSearchObj->mTransform->SetParent(mParentObj);
		//game->Debug()->Log("親検索");

		// 索敵オブジェのスクリプト取得
		mSearchScript = mPlayerSearchObj->GetScript<PlayerSearch>();
		// 大きさの変更
		auto setScale = XMVectorSet(
			objScale.x * mScalarX, objScale.y * mScalarY, objScale.z * mScalarZ, 0.0f);
		mPlayerSearchObj->mTransform->Scale(setScale);
		mSearchScript->SetScalarZ(mScalarZ);

		// 索敵範囲オブジェの位置設定
		auto childrenScale = mPlayerSearchObj->mTransform->Scale();
		// 位置の指定
		auto setPosition = XMVectorSet(0.0f, 0.0f, (objScale.z + childrenScale.z) / 2.0f, 0.0f);
		mSearchObjPosition = setPosition;

		// 位置の変更
		mPlayerSearchObj->mTransform->Position(-setPosition);
		//game->Debug()->Log(std::to_string(mPlayerSearchObj->mTransform->Position().z));

		// 敵の子供に追加する
		mPlayerSearchObj->mTransform->SetParent(mParentObj);

		mAddSearchObjCount = 1;
	}

	// 索敵オブジェの位置更新
	mPlayerSearchObj->mTransform->Position(-mSearchObjPosition);
	//if (searchScript->IsPlayerSearch()|| mIsAttckMode)
	if (mSearchScript->IsPlayerSearch()){
		if (!mIsDistanceAct) {
			mEnemyState = EnemyState::PlayerChase;
		}
		else {
			auto searchObjScale = mPlayerSearchObj->mTransform->Scale().z;

			int playerDistanceNumber = mSearchScript->PlayerDistance() / (searchObjScale / 3.0f);
			if (playerDistanceNumber == 3) {
				playerDistanceNumber = 2;
			}

			//game->Debug()->Log(std::to_string(playerDistanceNumber));
			mEnemyState = mDistanceVector[playerDistanceNumber];

			//// 配列にしてif文を無くす
			//if (mSearchScript->PlayerDistance() <= searchObjScale / 3.0f) {
			//	mEnemyState = EnemyState::PlayerShortDistance;
			//}
			//else if (mSearchScript->PlayerDistance() <= searchObjScale / 2.0f) {
			//	mEnemyState = EnemyState::PlayerCenterDistance;
			//}
			//else if (mSearchScript->PlayerDistance() > searchObjScale / 2.0f) {
			//	mEnemyState = EnemyState::PlayerLongDistance;
			//}
		}
	}
	else {
		mEnemyState = EnemyState::PlayerSearch;
	}
}

// プレイヤーの方向に回転します
void  Enemy::PlayerChaseMode() {
	// プレイヤーの捜索
	// プレイヤーの方向を向く
	auto playerObj = game->FindActor("Board");
	auto playerPosition = playerObj->mTransform->Position();
	// 親のステータスの取得
	auto parentPosition = mParentObj->mTransform->Position();
	
	auto v = parentPosition - playerPosition;
	auto angle = atan2(v.x, v.z);
	auto quaternion = XMQuaternionRotationAxis(mParentObj->mTransform->Up(), angle);
	//game->Debug()->Log(std::to_string(quaternion.x));
	//game->Debug()->Log(std::to_string(quaternion.z));
	if (playerPosition.x != parentPosition.x && playerPosition.z != parentPosition.z) {
		mParentObj->mTransform->Quaternion(XMQuaternionRotationAxis(mParentObj->mTransform->Up(), angle));
	}
	
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

// プレイヤーに当たったらノックバックします
void Enemy::KnockBack() {
	gameObject->mTransform->AddForce(mKnockBackHoukou * 10.0f);
}

// 敵の目の前にオブジェを生成します
void Enemy::SetForwardObj(Actor* setObj) {
	auto parentPosition = mParentObj->mTransform->Position();
	auto parentRotate = mParentObj->mTransform->Rotate();
	auto parentScale = gameObject->mTransform->Scale();
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
void Enemy::SetDamage(int damage) {
	mDamage = damage;
}

// リスポーンタイムです
void Enemy::SetResPawnTime(int time) {
	mResPawnTime = time;
	mInitResPawnTime = time;
}

// 一定距離まで落ちたらリスポーンします
void Enemy::ResPawnLine() {
	// 一定の位置まで落ちたらリスポーンタイムを減算
	if (mParentObj->mTransform->Position().y <= mResPawnHeigth) {

		if (mResPawnTime < 0) {
			mParentObj->mTransform->Position(mInitPosition);
			mParentObj->mTransform->Rotate(mInitRotate);
			mResPawnTime = mInitResPawnTime;
		}

		mResPawnTime--;
	}
}

// 敵の行動関数
void Enemy::Move() {

	auto collider = gameObject->GetComponent<PhysXColliderComponent>();
	auto colliderScale = collider->GetScale();
	//PlayerSearchMode(gameObject->mTransform->Scale());
	PlayerSearchMode(colliderScale);
	mPlayerSearchObj->mTransform->SetParent(mParentObj);

	//gameObject->mTransform->SetParent(mParentObj);

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
			//game->Debug()->Log("遠い");
			LongDistanceAttack();
		}
	}

	Enemy::PlayerColorChange();
	Enemy::ResPawnLine();
	

	//else {
	//	// 親の作成
	//	/*auto parentObj = game->CreateActor("Assets/ParentObj");
	//	game->AddObject(parentObj);
	//	gameObject->mTransform->SetParent(parentObj);*/

	//	mParentObj = gameObject->mTransform->GetParent();
	//	mParentObj->mTransform->Position(gameObject->mTransform->Position());
	//	mParentObj->mTransform->Rotate(gameObject->mTransform->Rotate());
	//	//mParentObj = gameObject->mTransform->GetParent();
	//	//mInitParentPositionY = mParentObj->mTransform->Position().y;
	//	// 初期ステータスを入れる
	//	mInitPosition = gameObject->mTransform->Position();
	//	mInitRotate = gameObject->mTransform->Rotate();
	//	// 子のステータスをリセット
	//	Enemy::ResetStatus();
	//	// CGの生成
	//	//Enemy::EnemyCGCreate();
	//	game->Debug()->Log("親生成");

	//	mParentCreateCount = 1;
	//}

	
}

// 敵グラフィックの生成
void Enemy::EnemyCGCreate() {
	//auto model = mEnemyCGObj->GetComponent<ModelComponent>();

	std::string createCGName = "Assets/Enemy/EnemyCGObj/" + gameObject->Name() + "CG";
	auto cgName = createCGName.c_str();

	mEnemyCGObj = game->CreateActor(cgName);
	game->AddObject(mEnemyCGObj);
	mEnemyCGObj->mTransform->SetParent(gameObject);
	/*game->AddObject(mEnemyCGObj);
	auto setCGScale = XMVectorSet(sizeX, sizeY, sizeZ, 0.0f);
	mEnemyCGObj->mTransform->Scale(setCGScale);
	mEnemyCGObj->mTransform->SetParent(gameObject);*/
}

// 敵のステータスの初期化
void Enemy::InitStatus() {
	gameObject->mTransform->Position(mInitPosition);
	gameObject->mTransform->Rotate(mInitRotate);
}

// 敵のステータスのリセット
void Enemy::ResetStatus() {
	auto ZeroStatus = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	gameObject->mTransform->Position(ZeroStatus);
	gameObject->mTransform->Rotate(ZeroStatus);
}

bool Enemy::IsDead() {
	return mIsDead;
}