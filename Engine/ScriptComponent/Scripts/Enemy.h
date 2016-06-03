
#pragma once
#include "main.h"
#include <vector>

class PlayerSearch;

enum class EnemyState {
	PlayerSearch,
	PlayerChase,
	PlayerShortDistance,
	PlayerCenterDistance,
	PlayerLongDistance
};

class Enemy :public IDllScriptComponent{
public:
	virtual ~Enemy();
	virtual void Initialize()override;
	virtual void Start()override;
	virtual void Update()override;
	virtual void Finish()override;
	virtual void OnCollideBegin(Actor* target)override;
	virtual void OnCollideEnter(Actor* target)override;
	virtual void OnCollideExit(Actor* target)override;
	// 敵の色の設定です
	virtual void PlayerColorChange();
	// 索敵移動します
	virtual void SearchMove();
	// 索敵関数です
	virtual void PlayerSearchMode(const XMVECTOR objScale);
	// プレイヤーの方向に回転します
	virtual void PlayerChaseMode();
	// プレイヤーを追跡します
	virtual void PlayerChase();
	// プレイヤーとの距離が短いときの行動です
	virtual void ShortDistanceAttack();
	// プレイヤーとの距離が中間の行動です
	virtual void CenterDistanceAttack();
	// プレイヤーとの距離が長いときの行動です
	virtual void LongDistanceAttack();
	// プレイヤーに当たったらノックバックします
	virtual void KnockBack();
	// 敵の目の前にオブジェを生成します
	void SetForwardObj(Actor* setObj);
	// ダメージの設定です
	void SetDamage(int damage);
	// リスポーンタイムです
	void SetResPawnTime(int time);
	// 一定距離まで落ちたらリスポーンします
	void ResPawnLine();
	// 敵の行動関数
	void Move();
	// 敵グラフィックの生成
	void EnemyCGCreate();
	// 敵のステータスの初期化
	void InitStatus();
	// 敵のステータスのリセット
	void ResetStatus();
	bool IsDead();

protected:
	//メンバ変数
	Actor* mPlayerSearchObj;				// 索敵範囲オブジェクト
	Actor* mParentObj;						// 空の親オブジェクト
	Actor* mEnemyCGObj;						// 敵のグラフィックオブジェクト
	PlayerSearch* mSearchScript;			// 索敵範囲オブジェクトのスクリプト
	EnemyState mEnemyState;					// enumクラスのEnemyState(敵の行動選択時に使用)
	XMVECTOR mSize;							// 敵の大きさ
	XMVECTOR mInitPosition;					// 初期位置
	XMVECTOR mInitRotate;;					// 初期の回転
	XMVECTOR mSearchObjPosition;			// 索敵範囲の位置
	XMVECTOR mKnockBackHoukou;
	int mAddSearchObjCount;					// 索敵範囲オブジェの作成カウント
	int mDamage;							// プレイヤーへのダメージ
	int mResPawnTime;						// リスポーンタイム
	int mInitResPawnTime;					// 初期リスポーンタイム
	int mParentCreateCount;					// 親の生成カウント
	int mCGCreateCount;						// CGの生成カウント(全部のリソースが揃ったら消去する)
	float mSpeed;							// 敵の速度変更
	float mHalfSizeZ;						// 敵のZの大きさの半分
	float mPositionY;						// 敵のYの位置（床との位置補正に使用）
	float mRotateY;							// 敵の回転角	
	float mScalarX;							// スケール値(X)
	float mScalarY;							// スケール値(Y)
	float mScalarZ;							// スケール値(Z)
	//float mInitParentPositionY;				// 親の初期位置(Y)
	const float mResPawnHeigth = -10.0f;	// リスポーンする高さ
	bool mIsFloorHit;						// 床と当たったか
	bool mIsImmortalBody;					// 不死身の敵か(未実装)
	bool mIsDistanceAct;					// 距離判定の行動をするか
	bool mIsAttckMode;						// 攻撃途中か(距離外に出た場合の攻撃中止の防止)
	bool mIsDead;							// プレイヤーと当たったか
	// 行動配列(未使用)
	typedef std::vector<EnemyState> DistanceVector;
	DistanceVector mDistanceVector;
};