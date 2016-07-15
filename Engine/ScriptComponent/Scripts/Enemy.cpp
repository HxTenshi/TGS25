#include "Enemy.h"
#include "EnemyManager.h"
#include "PlayerSearch.h"
#include "SailBoard.h"
#include "ParentObj.h"
#include "EnemyCG.h"
#include "CreateEnemyObj.h"
#include "MoveSmoke.h"
//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"
#include "Engine\DebugEngine.h"

Enemy::~Enemy() {
	//gameObject->mTransform->AllChildrenDestroy();
	//game->DestroyObject(mEnemyCGObj);
}

//生成時に呼ばれます（エディター中も呼ばれます）
void Enemy::Initialize(){

	mEnemyState = EnemyState::PlayerSearch;
	mEnemyDeadState = EnemyDeadState::BlowAwayDead;
	mAddSearchObjCount = 0;
	mDamage = 0;
	mResPawnTime = 0;
	mCGCreateCount = 0;
	mAnimationID = 0;
	mInitSetCount = 0;
	mTornadosCount = 1;
	mSpeed = 1.0f * 0.01f;
	mSize = gameObject->mTransform->Scale();
	mHalfSizeZ = mSize.z / 2.0f;
	mPositionY = gameObject->mTransform->Position().y;
	mRotateY = 0.0f;
	mScalarX = 1.0f;
	mScalarY = 2.0f;
	mScalarZ = 3.0f;
	mAddPlayerChaseStopDistance = 0.0f;
	mTornadoDistance = 50.0f;
	mBlowAwayY = 3.141593f / 5.0f;
	mBlowAwayPower = 20.0f;
	mBlowAwayInterval = 4.0f;
	mIsMove = true;
	mIsFloorHit = false;
	mIsCloudHit = false;
	mIsDamage = false;
	mIsImmortalBody = false;
	mIsDistanceAct = false;
	mIsKnockBackDirection = false;
	mIsAttckMode = false;
	mIsChaseRotate = true;
	mIsTornadoBlowAway = false;
	mIsPlayerHeal = false;
	mIsCreateChase = false;
	// 配列に追跡行動のenumクラスを入れる
	mDistanceVector.push_back(EnemyState::PlayerShortDistance);
	mDistanceVector.push_back(EnemyState::PlayerCenterDistance);
	mDistanceVector.push_back(EnemyState::PlayerLongDistance);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Enemy::Start(){
	// 親の取得	
	mParentObj = gameObject->mTransform->GetParent();
	//// 初期ステータスを入れる
	//mInitPosition = mParentObj->mTransform->Position();
	//mInitRotate = mParentObj->mTransform->Rotate();
	// エネミーマネージャーの作成
	mEnemyManagerObj = game->CreateActor("Assets/Enemy/EnemyManager");
	game->AddObject(mEnemyManagerObj);
	mEnemyManagerObj->mTransform->SetParent(gameObject);
	mEnemyManagerScript = mEnemyManagerObj->GetScript<EnemyManager>();
	// おじいちゃんである生成オブジェクトの捜索
	//mCreateEnemyObj = game->FindActor("CreateEnemyObj");
	mCreateEnemyObj = mParentObj->mTransform->GetParent();
	if (mCreateEnemyObj != nullptr) {
		if (mCreateEnemyObj->Name() == "CreateEnemyObj") {
			auto createEnemyScript = 
				mCreateEnemyObj->GetScript<CreateEnemyObj>();
			mIsCreateChase = createEnemyScript->GetIsPlayerChaseMode();
		}
	}
}

//毎フレーム呼ばれます
void Enemy::Update(){
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Enemy::Finish(){
}

//コライダーとのヒット時に呼ばれます
void Enemy::OnCollideBegin(Actor* target){
	if (mEnemyState != EnemyState::DeadMove) {
		if (target->Name() == "Board") {
			auto playerScript = target->GetScript<SailBoard>();
			// プレイヤーが無敵状態なら死亡
			// そうでない場合はプレイヤーにダメージを与える
			if (playerScript->IsUnrivaled()) {
				playerScript->Damage(-mDamage);
				// 死亡した瞬間に当たり判定をトリガーにする
				auto collider = gameObject->GetComponent<PhysXColliderComponent>();
				collider->SetIsTrigger(true);
				// プレイヤーの方に回転
				auto parentPosition = mParentObj->mTransform->Position();
				auto playerPosition = target->mTransform->Position();
				auto v = parentPosition - playerPosition;
				auto angle = atan2(v.x, v.z);
				auto quaternion = XMQuaternionRotationAxis(mParentObj->mTransform->Up(), angle);
				mEnemyState = EnemyState::DeadMove;
				// 死亡アニメーションの呼び出し
				Enemy::SetAnimationID(2);
				Enemy::SetAnimationLoop(false);
			}
			else {
				if (!mIsDamage) {
					playerScript->Damage(mDamage);
					mIsDamage = true;
				}
			}
			// サウンドの再生
			mEnemyManagerScript->EnemyPlaySound("hit");
		}
		// 竜巻に当たったら死亡する
		if (target->Name() == "Tornado") {
			mEnemyState = EnemyState::DeadMove;
			mEnemyDeadState = EnemyDeadState::TornadoDead;
			mBlowAwayPower = 100.0f;
			mBlowAwayTornadoObj = mTornadoObj;
			mTornadoObj = nullptr;
			// コライダーのトリガーをオンにする
			auto collider = gameObject->GetComponent<PhysXColliderComponent>();
			collider->SetIsTrigger(true);
		}
	}
	if (target->Name() == "Floor") {
		mInitSetCount = 1;
		mIsFloorHit = true;
		mIsDamage = false;
	}
	if (target->Name() == "Tower" ||
		target->Name() == "bridge" ||
		target->Name() == "Tree") {
		auto KnockBack = mParentObj->mTransform->Forward();
		auto parentPosition = mParentObj->mTransform->Position();
		mParentObj->mTransform->Position(parentPosition + KnockBack);
	}
	// 雲と当たったら消えるフラグを間接的にtrueにする
	if (target->Name() == "Cloud_s" ||
		target->Name() == "Cloud_s1" ||
		target->Name() == "Cloud_s2") {
		mIsCloudHit = true;
	}
}

//コライダーとのヒット中に呼ばれます
void Enemy::OnCollideEnter(Actor* target){
	if (target->Name() == "Floor") {
	
		// 床の角度と同じにする
		auto targetPosition = target->mTransform->Position();
		auto floorRotate = target->mTransform->Rotate();
		auto parentPosition = mParentObj->mTransform->Position();
		auto parentRotate = mParentObj->mTransform->Rotate();
		// 角度が０なら代入する
		if (abs(floorRotate.x) > abs(floorRotate.z)) {
			if (floorRotate.z == 0) floorRotate.z = floorRotate.x;
			// 角度の変更
			auto enemyRotate = XMVectorSet(
				floorRotate.x * cosf(parentRotate.y), parentRotate.y,
				floorRotate.z * -sinf(parentRotate.y), 0.0f);
			mParentObj->mTransform->Rotate(enemyRotate);
		}
		else if (abs(floorRotate.x) < abs(floorRotate.z)) {
			if (floorRotate.x == 0) floorRotate.x = floorRotate.z;
			// 角度の変更
			auto enemyRotate = XMVectorSet(
				floorRotate.x * -sinf(parentRotate.y), parentRotate.y,
				floorRotate.z * cosf(parentRotate.y), 0.0f);
			mParentObj->mTransform->Rotate(enemyRotate);
		}

		mIsFloorHit = true;
		mIsDamage = false;
	}

	// 竜巻に当たったら死亡する
	if (target->Name() == "Tornado" && mEnemyState != EnemyState::DeadMove) {
		mEnemyState = EnemyState::DeadMove;
		mEnemyDeadState = EnemyDeadState::TornadoDead;
		mBlowAwayPower = 100.0f;
		mBlowAwayTornadoObj = mTornadoObj;
		// コライダーのトリガーをオンにする
		auto collider = gameObject->GetComponent<PhysXColliderComponent>();
		collider->SetIsTrigger(true);
	}
}

//コライダーとのロスト時に呼ばれます
void Enemy::OnCollideExit(Actor* target){
	if (target->Name() == "Floor") mIsFloorHit = false;
}

// 索敵移動をします（デフォルト設定）
void Enemy::PlayerSearchMove() {
	// デフォルトの移動
	auto position = gameObject->mTransform->Position();
	auto forward = gameObject->mTransform->Forward();
	gameObject->mTransform->Position(position + forward);
}
// 索敵関数です
void Enemy::PlayerSearchMode(const XMVECTOR objScale) {
	// 死亡していたらすぐに返す
	if (mEnemyState == EnemyState::DeadMove) return;
	// 索敵範囲オブジェの生成後に追跡中止距離の設定を行う
	if (mAddSearchObjCount == 1) {
		// 追跡中止の距離の設定
		mSearchScript->AddChaseStopDistance(mAddPlayerChaseStopDistance);
		mAddSearchObjCount = 2;
	}
	// 索敵オブジェの作成
	if (mAddSearchObjCount < 1) {
		mPlayerSearchObj = game->CreateActor("Assets/Enemy/PlayerSearchRange");
		game->AddObject(mPlayerSearchObj);
		mPlayerSearchObj->mTransform->SetParent(mParentObj);
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
		// 敵の子供に追加する
		mPlayerSearchObj->mTransform->SetParent(mParentObj);
		// 生成時に追跡するか
		mSearchScript->SetIsPlayerHit(mIsCreateChase);
		//if (mIsCreateChase) mSearchScript->SetIsPlayerHit(mIsCreateChase);
		// CGの生成
		std::string baseName = "Assets/Enemy/EnemyCGObj/" + gameObject->Name() + "CG";
		auto createCGObjName = baseName.c_str();
		auto CGObj = game->CreateActor(createCGObjName);
		game->AddObject(CGObj);
		mEnemyCGObj = CGObj;
		mEnemyCGObj->mTransform->SetParent(mParentObj);
		mEnemyCGScript = CGObj->GetScript<EnemyCG>();
		// 一度生成したら二度と生成しない
		mAddSearchObjCount = 1;
	}
	// 索敵オブジェの位置更新
	mPlayerSearchObj->mTransform->Position(-mSearchObjPosition);
	// 索敵範囲内にプレイヤーが入ったら
	if (mSearchScript->IsPlayerSearch()){
		if (!mIsDistanceAct) {
			mEnemyState = EnemyState::PlayerChase;
			return;
		}
		else {
			if (!mIsAttckMode) {
				auto searchObjScale = mPlayerSearchObj->mTransform->Scale().z;
				// 追跡行動の数字を計算
				int playerDistanceNumber = mSearchScript->GetPlayerDistance() / (searchObjScale / 3.0f);
				if (playerDistanceNumber >= 3) {
					playerDistanceNumber = 2;
				}
				// 敵の追跡行動の決定
				mEnemyState = mDistanceVector[playerDistanceNumber];
				return;
			}
		}
	}
	else {
		if (!mIsAttckMode) {
			if (mSearchScript->IsLost()) {
				// 通常行動
				mEnemyState = EnemyState::PlayerSearch;
				// 戻る行動
				if(mCreateEnemyObj->Name() == "CreateEnemyObj")
					mEnemyState = EnemyState::ReturnMove;
				return;
			}
		}
	}
}

// プレイヤーの方向に回転します
void  Enemy::PlayerChaseMode(const float startPoint, const float endPoint) {
	if (!mIsFloorHit) return;
	// プレイヤーの捜索
	// プレイヤーの方向を向く
	auto playerObj = game->FindActor("Board");
	//if(playerObj != nullptr)
	auto playerPosition = playerObj->mTransform->Position();
	// 親のステータスの取得
	auto parentPosition = mParentObj->mTransform->Position();
	// プレイヤーとのベクトルの向きを計算
	auto v = parentPosition - playerPosition;
	auto angle = atan2(v.x, v.z);
	auto quaternion = XMQuaternionRotationAxis(mParentObj->mTransform->Up(), angle);
	// 二次元の距離を求める
	auto v2PlayerPosition = XMVectorSet(playerPosition.x, 0.0f, playerPosition.z, 0.0f);
	auto v2ParentPosition = XMVectorSet(parentPosition.x, 0.0f, parentPosition.z, 0.0f);
	auto playerDistance = XMVector3Length(v2PlayerPosition - v2ParentPosition);
	//コライダーの大きさを取得
	auto collider = gameObject->GetComponent<PhysXColliderComponent>();
	auto colliderScale = collider->GetScale();
	// プレイヤーとの距離がプレイヤーの一定の大きさ以下なら回転しない
	if (playerDistance.z >= colliderScale.z / 2.0f + startPoint)mIsChaseRotate = true;
	else if (playerDistance.z < colliderScale.z / 2.0f + endPoint)mIsChaseRotate = false;
	// mIsChaseRotate が true ならプレイヤーの方に回転
	// これにより、敵がプレイヤーを追跡するときに角ばった動きをしなくなる
	if (mIsChaseRotate)mParentObj->mTransform->Quaternion(quaternion);
}
// プレイヤーを追跡します（デフォルト設定）
void Enemy::PlayerChaseMove() {
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

// 竜巻から逃げるときの行動です
void Enemy::TornadoEscapeMove(Actor* tornadoObj) {
	if (tornadoObj == nullptr) return;
	if (tornadoObj->Name() != "Tornado") {
		mTornadoObj = nullptr;
		mTornadoMinDistance = mTornadoDistance;
		return;
	}
	// スモックを動かす
	mIsMove = true;
	// 竜巻の位置の取得
	auto tornadoPosition = tornadoObj->mTransform->Position();
	// 親のステータスの取得
	auto parentPosition = mParentObj->mTransform->Position();
	// 竜巻との回転方向を計算(逃げるようにする)
	auto v = parentPosition - tornadoPosition;
	auto angle = atan2(v.x, v.z);
	// angleに90°加算する
	auto escapeAngle = angle - 3.141593f / 2.0f;
	// escapeAngle が 0.000000f になっている状態でを足すとエラーな軸となる
	// ログ表示では escapeAngle が 0.000000f となっているが、0.000000fと認識してくれない
	// string型だと認識してくれる
	if (std::to_string(escapeAngle) == "0.000000") escapeAngle = 0.0f;
	if (escapeAngle < 0) escapeAngle += 3.141593f * 2.0f;
	// 竜巻から逃げるように移動する
	mParentObj->mTransform->Rotate(mParentObj->mTransform->Up() * (escapeAngle));
	mParentObj->mTransform->Position(
		parentPosition +
		((mParentObj->mTransform->Forward() +
			(-mParentObj->mTransform->Left() / mTornadoInterval))
			* (mTornadoPower * 0.01f)) * Enemy::GetEnemyDeltaTime(60.0f));
	auto distance = XMVector3Length(tornadoPosition - parentPosition);
	if (distance.x <= 10.0f) mTornadoInterval = 1.0f;
	auto tornadoCollider = mTornadoObj->GetComponent<PhysXColliderComponent>();
	if (distance.x <= tornadoCollider->GetScale().x) {
		mEnemyState = EnemyState::DeadMove;
		mEnemyDeadState = EnemyDeadState::TornadoDead;
		mBlowAwayPower = 100.0f;
		mBlowAwayTornadoObj = mTornadoObj;
		mTornadoObj = nullptr;
		// コライダーのトリガーをオンにする
		auto collider = gameObject->GetComponent<PhysXColliderComponent>();
		collider->SetIsTrigger(true);
	}

	Enemy::SetAnimationID(0);
}

// プレイヤーに当たったらノックバックします
void Enemy::KnockBack() {
	auto parentPosition = mParentObj->mTransform->Position();
	// ノックバックの方向を決める
	if (!mIsKnockBackDirection) {
		// プレイヤーの方に回転
		auto player = game->FindActor("Board");
		auto playerPosition = player->mTransform->Position();
		auto v2ParentPosition = XMVectorSet(parentPosition.x, 0.0f, parentPosition.z, 0.0f);
		auto v2PlayerPosition = XMVectorSet(playerPosition.x, 0.0f, playerPosition.z, 0.0f);
		auto v2 = v2ParentPosition - v2PlayerPosition;
		mKnockBackDIrection = XMVector3Normalize(v2);
		mIsKnockBackDirection = true;
	}
	// 後ろに仰け反って落ちる
	mParentObj->mTransform->Position(
		parentPosition + ((mKnockBackDIrection * 20.0f * 0.01f) -
		mParentObj->mTransform->Up() * 5.0f * 0.01f) * Enemy::GetEnemyDeltaTime(60.0f));
	Enemy::Dead();
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

// 索敵範囲のサイズを変更します
void Enemy::SetSearchRangeScale(const float scaleX, const float scaleY, const float scaleZ) {
	mScalarX = scaleX;
	mScalarY = scaleY;
	mScalarZ = scaleZ;
}

// 移動時のスモックオブジェを生成します
void Enemy::EnemyMoveSmoke() {
	// 床に触れていたら生成
	// 床に触れていなかったら削除する
	if (mIsFloorHit) {
		if (mIsMove) {
			if (mRightSmokeObj == nullptr) {
				// 子の生成 自分の名前から子を選択し生成します
				std::string baseName = "Assets/Enemy/" + gameObject->Name() + "MoveSmoke";
				auto createObjName = baseName.c_str();
				// 右のスモック生成
				mRightSmokeObj = game->CreateActor(createObjName);
				game->AddObject(mRightSmokeObj);
				mRightSmokeObj->mTransform->SetParent(gameObject);
				// 左のスモック生成
				mLeftSmokeObj = game->CreateActor(createObjName);
				game->AddObject(mLeftSmokeObj);
				mLeftSmokeObj->mTransform->SetParent(gameObject);
			}
			// コライダーで調整する
			auto collider = gameObject->GetComponent<PhysXColliderComponent>();
			auto colliderScale = collider->GetScale();
			auto enemyCGPosition = mEnemyCGObj->mTransform->Position();
			auto radius = 3.141593f / 180.0f;
			// 右のスモックのステータス設定
			auto rightX = -colliderScale.x / 2.0f - enemyCGPosition.x;
			auto rightZ = -colliderScale.z / 2.0f + enemyCGPosition.z;
			auto rightPosition = XMVectorSet(rightX, 0.3f, rightZ, 0.0f);
			mRightSmokeObj->mTransform->Position(rightPosition);
			// 左のスモックのステータス設定
			auto leftX = colliderScale.x / 2.0f + enemyCGPosition.x;
			auto leftPosition = XMVectorSet(leftX, 0.3f, rightZ, 0.0f);
			mLeftSmokeObj->mTransform->Position(leftPosition);
			mRightSmokeScript = mRightSmokeObj->GetScript<MoveSmoke>();
			mLeftSmokeScript = mLeftSmokeObj->GetScript<MoveSmoke>();
			mRightSmokeScript->SetSpeed(0.1f);
			mLeftSmokeScript->SetSpeed(0.1f);
		}
		else {
			if (mRightSmokeObj == nullptr)return;
			mRightSmokeScript->SetSpeed(0.0f);
			mLeftSmokeScript->SetSpeed(0.0f);
		}
	}
	else {
		if (mRightSmokeObj == nullptr)return;
		mRightSmokeScript->SetSpeed(0.0f);
		mLeftSmokeScript->SetSpeed(0.0f);
	}
}

// 一定距離まで落ちたらリスポーンします
void Enemy::ResPawnLine() {
	// 一定の位置まで落ちたらリスポーンタイムを減算
	if (mParentObj->mTransform->Position().y <= mResPawnHeigth) {
		// リスポーンタイムが０ならリスポーンする
		if (mResPawnTime < 0) {
			mEnemyManagerScript->InitStatus();
			mResPawnTime = mInitResPawnTime;
			mInitSetCount = 0;
		}
		mResPawnTime -= Enemy::GetEnemyDeltaTime(60.0f);
	}
}

// 竜巻を探します
void Enemy::SearchTornado() {
	if (mEnemyState == EnemyState::DeadMove) return;
	// 竜巻の親検索
	auto tornadoParent = game->FindActor("Tornados");
	if (tornadoParent != nullptr) {
		auto tornadoChildren = tornadoParent->mTransform->Children();
		// 子供がいるなら竜巻との距離を求める
		auto size = tornadoChildren.size();
		// 竜巻が消えたなら、一回エラー防止にnullptrを入れる
		if (mTornadosCount != size) {
			mTornadosCount = size;
			mEnemyState = EnemyState::PlayerSearch;
			mTornadoObj = nullptr;
			mTornadoMinDistance = mTornadoDistance;
			return;
		}
		if (size >= 1) {
			// 子の中から最短距離の竜巻を探す
			for (auto i = tornadoChildren.begin(); i != tornadoChildren.end(); ++i) {
				// 現在のiの竜巻を入れる
				auto tathumaki = *i;
				auto tathumakiPosition = XMVectorSet(
					tathumaki->mTransform->Position().x, 0.0f,
					tathumaki->mTransform->Position().z, 0.0f);
				auto parentPosition = XMVectorSet(
					mParentObj->mTransform->Position().x, 0.0f,
					mParentObj->mTransform->Position().z, 0.0f);
				// iに入っている竜巻の距離を計算する
				auto tornadoDistance = XMVector3Length(tathumakiPosition - parentPosition);
				// 最短距離の竜巻だったら入れる
				if (tornadoDistance.x <= mTornadoMinDistance) {
					mTornadoMinDistance = tornadoDistance.x;
					mTornadoObj = tathumaki;
				}
			}
			// 範囲内なら竜巻に巻き込まれる行動を行う
			if (mTornadoMinDistance <= mTornadoDistance)
				mEnemyState = EnemyState::TornadoEscape;
			else mEnemyState = EnemyState::PlayerSearch;
		}
		else {
			// 竜巻がないなら通常行動をする
			if (mEnemyState != EnemyState::DeadMove) {
				mTornadoObj = nullptr;
				mTornadoMinDistance = mTornadoDistance;
			}
		}
	}
}

// 敵の死亡行動です
void Enemy::DeadMove() {
	// 死亡行動の状態
	switch (mEnemyDeadState)
	{
	case EnemyDeadState::KnockBackDead: Enemy::KnockBack(); break;
	case EnemyDeadState::BlowAwayDead: Enemy::DeadBlowAwayMove(); break;
	case EnemyDeadState::TornadoDead: Enemy::DeadTornadoBlowAwayMove(); break;
	}
}

// 吹き飛び時の死亡行動です
void Enemy::DeadBlowAwayMove() {
	// 当たりモーション
	auto position = mParentObj->mTransform->Position();
	// 180度まで回転
	if (mParentObj->mTransform->Rotate().x < 3.141593f) {
		// 回転
		auto rotate = mParentObj->mTransform->Rotate();
		rotate.x += 3.141593f / (180.0f / mBlowAwayInterval);
		mParentObj->mTransform->Rotate(rotate);
	}
	else {
		// 回転しきって条件を満たしたら死亡
		Enemy::Dead();
	}
	// 上昇
	auto up = mParentObj->mTransform->Up() *
		cosf(gameObject->mTransform->Rotate().x) * 0.01f * mBlowAwayPower;
	mParentObj->mTransform->Position(position + up * Enemy::GetEnemyDeltaTime(60.0f));
}

// 竜巻に巻き込まれた時の死亡行動です
void Enemy::DeadTornadoMove() {
	// 上昇
	// スモックを止める
	mIsMove = false;
	// 竜巻がなくなっていたらその場で吹き飛ばす
	// 死亡した場合はnullptr参照が出来ないのでこうする 一回アクセスエラーとなる
	if (mBlowAwayTornadoObj->Name() != "Tornado") mIsTornadoBlowAway = true;
	// 竜巻のコライダーの取得
	auto tornadoCollider = mBlowAwayTornadoObj->GetComponent<PhysXColliderComponent>();
	auto tornadoColliderScale = tornadoCollider->GetScale();
	// 竜巻に沿って回転
	auto tornadoPosition = mBlowAwayTornadoObj->mTransform->Position();
	// 親のステータスの取得
	auto parentPosition = mParentObj->mTransform->Position();
	// 竜巻の高さ以上になったら別方向に吹き飛ばす
	if (parentPosition.y >= tornadoColliderScale.y) {
		mIsTornadoBlowAway = true;
	}
	// 条件を満たしたら別方向に吹き飛ばす
	if (!mIsTornadoBlowAway) {
		// ワールドのY座標で上昇する
		auto vectorY = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		auto up = vectorY * cosf(mParentObj->mTransform->Rotate().x) * 0.01f * mBlowAwayPower;
		// 竜巻との回転方向を計算(逃げる方向と一緒にする)
		auto v = parentPosition - tornadoPosition;
		auto angle = atan2(v.x, v.z);
		auto escapeAngle = angle - 3.141593f / 2.0f;
		// 回転補正
		if (std::to_string(escapeAngle) == "0.000000") escapeAngle = 0.0f;
		if (escapeAngle < 0) escapeAngle += 3.141593f * 2.0f;
		// 竜巻のように回転しながら吹き飛ぶ
		mParentObj->mTransform->Rotate(mParentObj->mTransform->Up() * (escapeAngle));
		mParentObj->mTransform->Position(
			parentPosition +
			((up * mTornadoUpPower * 0.01f) +
				(mParentObj->mTransform->Forward() -
					(mParentObj->mTransform->Left() / (mTornadoRotateScale))) *
				(mTornadoRotatePower * 0.01f)) * Enemy::GetEnemyDeltaTime(60.0f));

		mEnemyCGObj->mTransform->Rotate(mParentObj->mTransform->Left() * 1.0f);
		mTornadoRotateScale += mAddTornadoRotateScale;
	}
	// 死亡アニメーション
	Enemy::SetAnimationID(2);
	if (Enemy::GetAnimationTime() > 10.0f) {
		Enemy::SetAnimationTime(10.0f);
	}
}

// 竜巻に吹き飛ばされる時の死亡行動です
void Enemy::DeadTornadoBlowAwayMove() {
	if (!mIsTornadoBlowAway) {
		// 竜巻に吹き飛ばされた時のモーション
		Enemy::DeadTornadoMove();
	}
	else {
		auto parentPosition = mParentObj->mTransform->Position();
		mParentObj->mTransform->Position(
			parentPosition +
			(mParentObj->mTransform->Forward() +
				(mParentObj->mTransform->Up() * cos(mBlowAwayY)))
			* Enemy::GetEnemyDeltaTime(60.0f));
		if (mBlowAwayY < 3.141593f) {
			mBlowAwayY += (3.141593f / 180.0f) * Enemy::GetEnemyDeltaTime(60.0f);
			if (mBlowAwayY > 3.141593f) mBlowAwayY = 3.141593f;
		}
		// 死亡判定
		Enemy::Dead();
	}
}

// 敵が生成オブジェクトに戻る行動です
void Enemy::ReturnMove() {
	// 生成オブジェクトがなかったらすぐに通常行動に戻す
	if (mCreateEnemyObj == nullptr) {
		mEnemyState = EnemyState::PlayerSearch;
		return;
	}
	mIsMove = true;

	auto createEnemyPosition = mCreateEnemyObj->mTransform->Position();
	// 親のステータスの取得
	auto parentPosition = mParentObj->mTransform->Position();
	// 生成オブジェクトとの距離の計算
	auto distance = XMVector3Length(parentPosition - createEnemyPosition);
	auto createEnemyScript = mCreateEnemyObj->GetScript<CreateEnemyObj>();
	// 一定距離まで近づいたら通常行動に戻す
	if (createEnemyScript->GetReturnDistance() <= distance.x) {
		// 生成オブジェクトとのベクトルの向きを計算
		auto v = parentPosition - createEnemyPosition;
		auto angle = atan2(v.x, v.z);
		auto quaternion = XMQuaternionRotationAxis(
			mParentObj->mTransform->Up(), angle);
		// 生成オブジェクトの方向を向く
		mParentObj->mTransform->Quaternion(quaternion);
		// 前方に移動
		auto forward = mParentObj->mTransform->Forward() * mSpeed * 0.01f;
		mParentObj->mTransform->Position(parentPosition - forward);
	}
	else {
		mEnemyState = EnemyState::PlayerSearch;
	}
}

// プレイヤーボートの耐久力回復処理を行います
void Enemy::PlayerHeal() {
	// プレイヤーの捜索
	auto player = game->FindActor("Board");
	// プレイヤーがいなければ死亡判定を優先する
	if (player->Name() != "Board") return;
	auto playerScript = player->GetScript<SailBoard>();
	playerScript->Damage(-mDamage);
}

// 死亡処理を行います
void Enemy::Dead() {
	auto parentPosition = mParentObj->mTransform->Position();
	if (!mIsPlayerHeal) {
		// 回復処理
		Enemy::PlayerHeal();
		mIsPlayerHeal = true;
		if(mEnemyDeadState != EnemyDeadState::TornadoDead)
			mEnemyManagerScript->EnemyPlaySound("heal");
	}
	else {
		// 雲に当たったら消滅 または リスポーン位置まで落ちたら消滅
		if (mIsCloudHit || parentPosition.y <= mResPawnHeigth || parentPosition.y >= 300.0f) {
			game->DestroyObject(mPlayerSearchObj);
			game->DestroyObject(mEnemyCGObj);
			game->DestroyObject(gameObject);
			game->DestroyObject(gameObject->mTransform->GetParent());
		}
	}
}

// 敵の行動関数
void Enemy::Move() {
	/*auto collider2 = gameObject->GetComponent<PhysXColliderComponent>();
	auto collider2Scale = collider2->GetScale();
	auto rayHitObj = game->PhysX()->Raycast(
		gameObject->mTransform->Up() * collider2Scale.y / 1.5f,
		gameObject->mTransform->Up() * -10.0f, 10.0f);
	if (rayHitObj) game->Debug()->Log(std::to_string(rayHitObj->mTransform->Position().y));*/
	// 索敵範囲の設定
	auto collider = gameObject->GetComponent<PhysXColliderComponent>();
	auto colliderScale = collider->GetScale();
	PlayerSearchMode(colliderScale);
	mPlayerSearchObj->mTransform->SetParent(mParentObj);
	// 竜巻を探す
	Enemy::SearchTornado();
	//// 竜巻の範囲内に入っていなかったら通常行動を行う
	// スイッチを使った行動
	switch (mEnemyState)
	{
	case EnemyState::TornadoEscape: Enemy::TornadoEscapeMove(mTornadoObj); break;
	case EnemyState::PlayerSearch: PlayerSearchMove(); break;
	case EnemyState::PlayerChase: PlayerChaseMove(); break;
	case EnemyState::PlayerShortDistance: ShortDistanceAttack(); break;
	case EnemyState::PlayerCenterDistance: CenterDistanceAttack(); break;
	case EnemyState::PlayerLongDistance: LongDistanceAttack(); break;
	case EnemyState::DeadMove: Enemy::DeadMove(); break;
	case EnemyState::ReturnMove: Enemy::ReturnMove(); break;
	}
	// リスポーンするかの判定
	Enemy::ResPawnLine();
	// ステータスのリセット
	mEnemyManagerScript->ResetStatus();
	// スモークの呼び出し
	Enemy::EnemyMoveSmoke();
}

// 敵の状態の変更を行います
void Enemy::ChangeEnemyState(EnemyState state) {
	mEnemyState = state;
}

// 敵の死亡状態の変更を行います
void Enemy::ChangeEnemyDeadState(EnemyDeadState state) {
	mEnemyDeadState = state;
}

// プレイヤーの追跡を中止する距離の加算です
void Enemy::AddPlayerChaseStopDistance(float distance) {
	mAddPlayerChaseStopDistance = distance;
}

// 竜巻のステータスを入れます
void Enemy::SetTornadoStatus(
	const float power, const float rotate, const float addRotate,
	const float rotatePower, const float upPower, const float interval,
	const float distance) {
	mTornadoPower = power;
	mTornadoRotateScale = rotate;
	mAddTornadoRotateScale = addRotate;
	mTornadoRotatePower = rotatePower;
	mTornadoUpPower = upPower;
	mTornadoInterval = interval;
	mTornadoDistance = distance;
}

// 子供側からアニメーションのIDを変えます
void Enemy::SetAnimationID(int id) {
	mEnemyCGScript->SetAnimationID(id);
}

// 子供側からアニメーションのタイムを変更します
void Enemy::SetAnimationTime(float time) {
	mEnemyCGScript->SetAnimationTime(time);
}

// アニメーションのタイムスケールを変更します
void Enemy::SetAnimationTimeScale(float timeScale) {
	mEnemyCGScript->SetTimeScale(timeScale);
}

// 子供側からアニメーションのループを変更します
void Enemy::SetAnimationLoop(bool isLoop) {
	mEnemyCGScript->SetLoop(isLoop);
}

// 現在のアニメーションのタイムを取得します
float Enemy::GetAnimationTime() {
	return mEnemyCGScript->GetAnimationTime();
}

// デルタタイムを取得します
float Enemy::GetEnemyDeltaTime(float framerate) {
	auto deltaTime = game->DeltaTime()->GetDeltaTime();
	return deltaTime * framerate;
}