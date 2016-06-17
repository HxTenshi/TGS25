#include "Enemy.h"
#include "PlayerSearch.h"
#include "SailBoard.h"
#include "ParentObj.h"
#include "EnemyCG.h"
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
	mAddSearchObjCount = 0;
	mDamage = 0;
	mResPawnTime = 0;
	mCGCreateCount = 0;
	mAnimationID = 0;
	mInitSetCount = 0;
	mSpeed = 1.0f * 0.01f;
	mSize = gameObject->mTransform->Scale();
	mHalfSizeZ = mSize.z / 2.0f;
	mPositionY = gameObject->mTransform->Position().y;
	mRotateY = 0.0f;
	mScalarX = 1.0f;
	mScalarY = 2.0f;
	mScalarZ = 3.0f;
	mAddPlayerChaseStopDistance = 0.0f;
	mTornadoDistance = 0.0f;
	mBlowAwayY = 3.141593f / 5.0f;
	mBlowAwayPower = 20.0f;
	mBlowAwayInterval = 4.0f;
	mIsMove = true;
	mIsFloorHit = false;
	mIsCloudHit = false;
	mIsImmortalBody = false;
	mIsDistanceAct = false;
	mIsBlowAway = true;
	mIsKnockBackDirection = false;
	mIsAttckMode = false;
	mIsChaseRotate = true;
	mIsTornadoRange = false;
	mIsTornadoBlowAway = false;
	mIsDead = false;
	// 配列に追跡行動のenumクラスを入れる
	mDistanceVector.push_back(EnemyState::PlayerShortDistance);
	mDistanceVector.push_back(EnemyState::PlayerCenterDistance);
	mDistanceVector.push_back(EnemyState::PlayerLongDistance);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Enemy::Start(){
	// 親の取得	
	mParentObj = gameObject->mTransform->GetParent();
	// 初期ステータスを入れる
	mInitPosition = mParentObj->mTransform->Position();
	mInitRotate = mParentObj->mTransform->Rotate();
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

		//// サウンドの再生
		//Enemy::EnemyPlaySound("hit");
		// 死亡した瞬間に当たり判定をトリガーにする
		auto collider = gameObject->GetComponent<PhysXColliderComponent>();
		collider->SetIsTrigger(true);
		// プレイヤーの方に回転
		auto parentPosition = mParentObj->mTransform->Position();
		auto playerPosition = target->mTransform->Position();
		auto v = parentPosition - playerPosition;
		auto angle = atan2(v.x, v.z);
		auto quaternion = XMQuaternionRotationAxis(mParentObj->mTransform->Up(), angle);
		// ダメージを与えて自分を消す(仮設定)
		playerScript->Damage(mDamage);
		mIsDead = true;
		// 死亡アニメーションの呼び出し
		Enemy::SetAnimationID(2);
		Enemy::SetAnimationLoop(false);
	}
	// 竜巻に当たったら死亡する
	if (target->Name() == "Tornado") {
		mIsDead = true;
		mIsBlowAway = true;
		mBlowAwayPower = 100.0f;
		// コライダーのトリガーをオンにする
		auto collider = gameObject->GetComponent<PhysXColliderComponent>();
		collider->SetIsTrigger(true);
		//mBlowAwayInterval = 1.0f;
	}

	if (target->Name() == "Floor") {
		mInitSetCount = 1;
		mIsFloorHit = true;
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
	if (target->Name() == "Floor") mIsFloorHit = true;
}

//コライダーとのロスト時に呼ばれます
void Enemy::OnCollideExit(Actor* target){
	if (target->Name() == "Floor") mIsFloorHit = false;
}
// 敵の色の設定です(デバッグ用)
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
	//auto color = mate->GetMaterial();
	//auto albedoColor = mate->mAlbedo;
	//auto color = XMVectorSet(albedoColor.x, albedoColor.y, .)
	//if(mate) mate->SetAlbedoColor()
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

		// CGの生成
		std::string baseName = "Assets/Enemy/EnemyCGObj/" + gameObject->Name() + "CG";
		auto createCGObjName = baseName.c_str();
		auto CGObj = game->CreateActor(createCGObjName);
		game->AddObject(CGObj);
		mEnemyCGObj = CGObj;
		//CGObj->mTransform->SetParent(gameObject);
		CGObj->mTransform->SetParent(mParentObj);
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
			}
		}
	}
	else {
		if (!mIsAttckMode) {
			mEnemyState = EnemyState::PlayerSearch;
		}
	}
}

// プレイヤーの方向に回転します
void  Enemy::PlayerChaseMode(const float startPoint, const float endPoint) {
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

// 竜巻から逃げるときの行動です
void Enemy::TornadoEscapeMove(Actor* tornadoObj) {
	mIsMove = true;
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
	// if (std::to_string(escapeAngle) == "0.000000") game->Debug()->Log("回転軸エラー");
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
	if (distance.x <= 8.0f) mTornadoInterval = 2.0f;

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

// 敵の目の前にオブジェを生成します(親がいる場合)
void Enemy::SetParentForwardObj(Actor* setObj) {
	auto parentPosition = mParentObj->mTransform->Position();
	auto parentRotate = mParentObj->mTransform->Rotate();
	auto collider = gameObject->GetComponent<PhysXColliderComponent>();
	auto colliderScale = collider->GetScale();
	auto setObjScale = setObj->mTransform->Scale();
	// 位置の指定
	auto setPosition = XMVectorSet(
		((colliderScale.z / 2.0f) + (setObjScale.z / 2.0f)) * sinf(parentRotate.y),
		0.0f,
		((colliderScale.z / 2.0f) + (setObjScale.z / 2.0f)) * cosf(parentRotate.y), 0.0f);
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
			auto rightPosition = XMVectorSet(rightX, 0.0f, rightZ, 0.0f);
			mRightSmokeObj->mTransform->Position(rightPosition);
			// 左のスモックのステータス設定
			auto leftX = colliderScale.x / 2.0f + enemyCGPosition.x;
			auto leftPosition = XMVectorSet(leftX, 0.0f, rightZ, 0.0f);
			mLeftSmokeObj->mTransform->Position(leftPosition);
			mRightSmokeScript = mRightSmokeObj->GetScript<MoveSmoke>();
			mLeftSmokeScript = mLeftSmokeObj->GetScript<MoveSmoke>();
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
			Enemy::InitStatus();
			mResPawnTime = mInitResPawnTime;
			mInitSetCount = 0;
		}
		mResPawnTime--;
	}
}

// 竜巻を探します
void Enemy::SearchTornado() {
	// 竜巻の親検索
	auto tornadoParent = game->FindActor("Tornados");
	if (mTornadoMinDistance == 0.0f) mTornadoMinDistance = mTornadoDistance;
	if (tornadoParent != nullptr) {
		auto tornadoChildren = tornadoParent->mTransform->Children();
		// 子供がいるなら竜巻との距離を求める
		auto size = tornadoChildren.size();
		if (size >= 2) {
			// 子の中から最短距離の竜巻を探す
			for (std::list<Actor*>::iterator i = tornadoChildren.begin(); i != tornadoChildren.end(); i++) {
				auto tathumaki = *i;
				// 竜巻との距離の計算
				auto tornadoDistance = XMVector3Length(
					tathumaki->mTransform->Position() -
					mParentObj->mTransform->Position());
				// 最短距離なら更新する
				if (tornadoDistance.x <= mTornadoMinDistance) {
					// 最短距離を入れる
					mTornadoMinDistance = tornadoDistance.x;
					// 竜巻オブジェを入れる
					mTornadoObj = tathumaki;
				}
			}
			// 竜巻との距離が一定距離内なら竜巻の方向に移動する
			if (mTornadoMinDistance <= mTornadoDistance) {
				mIsTornadoRange = true;
			}
		}
		else if (size == 1) {
			auto tathumaki = game->FindActor("Tornado");
			if (tathumaki != nullptr) {
				auto distance = XMVector3Length(
					tathumaki->mTransform->Position() - mParentObj->mTransform->Position());
				// 竜巻との距離が一定距離内なら竜巻の方向に移動する
				if (distance.x <= mTornadoDistance) {
					mIsTornadoRange = true;
					mTornadoObj = tathumaki;
				}
			}
			else {
				// 竜巻と当たっていなくて、途中で竜巻がなくなったら通常行動に戻す
				mIsTornadoRange = false;
				mTornadoObj = nullptr;
				mTornadoMinDistance = mTornadoDistance;
			}
		}
		else if (size == 0) {
			// 竜巻がないなら通常行動をする
			mIsTornadoRange = false;
			mTornadoObj = nullptr;
			mTornadoMinDistance = mTornadoDistance;
		}
	}
}

// 敵の死亡行動です
void Enemy::DeadMove() {
	auto position = mParentObj->mTransform->Position();
	// 吹き飛ぶ場合
	if (mIsBlowAway) {
		// 竜巻の吹き飛び
		if (mIsTornadoRange) {
			// 上昇
			// 竜巻がなくなっていたらその場で吹き飛ばす
			if (mTornadoObj == nullptr) {
				mIsTornadoBlowAway = true;
			}
			// 竜巻のコライダーの取得
			auto tornadoCollider = mTornadoObj->GetComponent<PhysXColliderComponent>();
			auto tornadoColliderScale = tornadoCollider->GetScale();
			// 竜巻に沿って回転
			auto tornadoPosition = mTornadoObj->mTransform->Position();
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
			else {
				mParentObj->mTransform->Position(
					parentPosition + 
					(mParentObj->mTransform->Forward() +
					(mParentObj->mTransform->Up() * cos(mBlowAwayY))) * Enemy::GetEnemyDeltaTime(60.0f));
				if (mBlowAwayY < 3.141593f) {
					mBlowAwayY += 3.141593f / 180.0f;
				}

				Enemy::Dead();
			}

			Enemy::SetAnimationID(2);
			//Enemy::SetAnimationID(false);
			if (Enemy::GetAnimationTime() > 10.0f) {
				Enemy::SetAnimationTime(10.0f);
			}
		}
		else {
			// 当たりモーション
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
	}
	else {
		// ノックバックモーション
		Enemy::KnockBack();
	}
	// 再度触れてもダメージ無し
	Enemy::SetDamage(0);
}

// 死亡処理を行います
void Enemy::Dead() {
	auto parentPosition = mParentObj->mTransform->Position();
	// 雲に当たったら消滅
	if (mIsCloudHit) {
		game->DestroyObject(mPlayerSearchObj);
		game->DestroyObject(mEnemyCGObj);
		game->DestroyObject(gameObject);
		game->DestroyObject(gameObject->mTransform->GetParent());
	}
	else {
		// リスポーン位置まで落ちたら消滅
		if (parentPosition.y <= mResPawnHeigth) {
			game->DestroyObject(mPlayerSearchObj);
			game->DestroyObject(mEnemyCGObj);
			game->DestroyObject(gameObject);
			game->DestroyObject(gameObject->mTransform->GetParent());
		}
	}
}

// 敵の行動関数
void Enemy::Move() {
	// 竜巻を探す
	Enemy::SearchTornado();
	// 死んでいなかったら各敵の行動を行う
	if (!mIsDead) {
		auto collider = gameObject->GetComponent<PhysXColliderComponent>();
		auto colliderScale = collider->GetScale();
		PlayerSearchMode(colliderScale);
		mPlayerSearchObj->mTransform->SetParent(mParentObj);
		// 竜巻の範囲内に入っていなかったら通常行動を行う
		if (!mIsTornadoRange) {
			if (mEnemyState == EnemyState::PlayerSearch) {
				// プレイヤーを捜索する行動
				SearchMove();
			}
			else {
				if (!mIsDistanceAct) {
					// プレイヤーを見つけたときの行動
					if (mEnemyState == EnemyState::PlayerChase) {
						PlayerChase();
					}
				}
				else {
					// 距離での行動
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
			}
		}
		else {
			// 竜巻に巻き込まれたときの行動
			Enemy::TornadoEscapeMove(mTornadoObj);
		}
		// リスポーンするかの判定
		Enemy::ResPawnLine();
	}
	else {
		// 死亡行動の呼び出し
		Enemy::DeadMove();
	}
	// ステータスのリセット
	Enemy::ResetStatus();
	// スモークの呼び出し
	Enemy::EnemyMoveSmoke();
}

// 親のステータスの初期化
void Enemy::InitStatus() {
	mParentObj->mTransform->Position(mInitPosition);
	mParentObj->mTransform->Rotate(mInitRotate);
}

// 敵のステータスのリセット
void Enemy::ResetStatus() {
	auto ZeroStatus = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	// ０の値に変える
	gameObject->mTransform->Position(ZeroStatus);
	gameObject->mTransform->Rotate(ZeroStatus);
}

// プレイヤーの追跡を中止する距離の加算です
void Enemy::AddPlayerChaseStopDistance(float distance) {
	mAddPlayerChaseStopDistance = distance;
}

// サウンドを再生します
void Enemy::EnemyPlaySound(const std::string soundName) {
	std::string playSoundName = "Assets/" + soundName;
	//// サウンドを鳴らす
	auto sound = gameObject->GetComponent<SoundComponent>();
	if (sound) return;
	/*sound->LoadFile("playSoundName");
	sound->Play();*/
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

// プレイヤーと指定されたオブジェの位置との距離を計算して返します
float Enemy::GetPlayerDistance(Actor* playerObj, Actor* otherObj) {
	auto playerPosition = playerObj->mTransform->Position();
	auto otherPosition = otherObj->mTransform->Position();
	// プレイヤーとの距離を計算
	auto distance = XMVector3Length(playerPosition - otherPosition);
	return distance.z;
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