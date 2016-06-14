
#pragma once
#include "main.h"
#include <vector>

class PlayerSearch;
class EnemyCG;

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
	// 敵の色の設定です(デバッグ用)
	virtual void PlayerColorChange();
	// 索敵移動します
	virtual void SearchMove();
	// 索敵関数です
	virtual void PlayerSearchMode(const XMVECTOR objScale);
	// プレイヤーの方向に回転します
	virtual void PlayerChaseMode(const float startPoint, const float endPoint);
	// プレイヤーを追跡します
	virtual void PlayerChase();
	// プレイヤーとの距離が短いときの行動です
	virtual void ShortDistanceAttack();
	// プレイヤーとの距離が中間の行動です
	virtual void CenterDistanceAttack();
	// プレイヤーとの距離が長いときの行動です
	virtual void LongDistanceAttack();
	// 竜巻から逃げるときの行動です
	virtual void TornadoEscapeMove(Actor* tornadoObj);
	// プレイヤーに当たったらノックバックします
	virtual void KnockBack();
	// 敵の目の前にオブジェを生成します(親がいる場合)
	void SetParentForwardObj(Actor* setObj);
	// ダメージの設定です
	void SetDamage(int damage);
	// リスポーンタイムです
	void SetResPawnTime(int time);
	// 索敵範囲のサイズを変更します
	void SetSearchRangeScale(const float scaleX, const float scaleY, const float scaleZ);
	// 移動時のスモックオブジェを生成します
	void EnemyMoveSmoke();
	// 一定距離まで落ちたらリスポーンします
	void ResPawnLine();
	// 敵の死亡行動です
	void DeadMove();
	// 死亡処理を行います
	void Dead();
	// 敵の行動関数
	void Move();
	// 親のステータスを初期化します
	void InitStatus();
	// 敵のステータスをリセットします
	void ResetStatus();
	// プレイヤーの追跡を中止する距離の加算です
	void AddPlayerChaseStopDistance(float distance);
	// 竜巻のステータスを入れます
	void SetTornadoStatus(
		const float power, const float rotate, const float addRotate,
		const float rotatePower, const float upPower,const float distance);
	// プレイヤーと指定されたオブジェの位置との距離を計算して返します
	float GetPlayerDistance(Actor* playerObj, Actor* otherObj);
	// 子供側からアニメーションのIDを変えます
	void SetAnimationID(int id);
	// 子供側からアニメーションのタイムを変更します(未使用)
	void SetAnimationTime(float time);
	// 子供側からアニメーションのタイムスケールを変更します
	void SetAnimationTimeScale(float timeScale);
	// 子供側からアニメーションのループを変更します
	void SetAnimationLoop(bool isLoop);
	// 現在のアニメーションのタイムを取得します
	float GetAnimationTime();
	// デルタタイムを取得します
	float GetEnemyDeltaTime(float framerate);

protected:
	//メンバ変数
	Actor* mPlayerSearchObj;				// 索敵範囲オブジェクト
	Actor* mParentObj;						// 空の親オブジェクト
	Actor* mEnemyCGObj;						// 敵のグラフィックオブジェクト
	Actor* mTornadoObj;						// 竜巻のオブジェクト
	Actor* mRightSmokeObj;
	Actor* mLeftSmokeObj;
	PlayerSearch* mSearchScript;			// 索敵範囲オブジェクトのスクリプト
	EnemyCG* mEnemyCGScript;				// 敵のCGオブジェクトのスクリプト
	EnemyState mEnemyState;					// enumクラスのEnemyState(敵の行動選択時に使用)
	XMVECTOR mSize;							// 敵の大きさ
	XMVECTOR mInitPosition;					// 初期位置
	XMVECTOR mInitRotate;;					// 初期の回転
	XMVECTOR mSearchObjPosition;			// 索敵範囲の位置
	XMVECTOR mKnockBackDIrection;			// ノックバックの方向
	int mAddSearchObjCount;					// 索敵範囲オブジェの作成カウント
	int mDamage;							// プレイヤーへのダメージ
	int mResPawnTime;						// リスポーンタイム
	int mInitResPawnTime;					// 初期リスポーンタイム
	int mCGCreateCount;						// CGの生成カウント(全部のリソースが揃ったら消去する)
	int mAnimationID;						// アニメーションのID
	int mInitSetCount;						// 最初に設定したか
	float mSpeed;							// 敵の速度変更
	float mHalfSizeZ;						// 敵のZの大きさの半分
	float mPositionY;						// 敵のYの位置（床との位置補正に使用）
	float mRotateY;							// 敵の回転角
	float mScalarX;							// スケール値(X)
	float mScalarY;							// スケール値(Y)
	float mScalarZ;							// スケール値(Z)
	float mAddPlayerChaseStopDistance;		// 追跡中止の距離に加算する値
	float mTornadoPower;					// 竜巻の吸い込む力
	float mTornadoRotateScale;				// 竜巻に回転する大きさ
	float mAddTornadoRotateScale;			// 竜巻への回転の加算
	float mTornadoRotatePower;				// 竜巻の回転力
	float mTornadoUpPower;					// 竜巻による上昇力
	float mTornadoDistance;					// 竜巻に反応する距離
	float mBlowAwayY;						// 竜巻に吹っ飛んだときのYの角度
	float mBlowAwayPower;					// 吹き飛ぶ速度
	float mBlowAwayInterval;				// 吹き飛び間隔
	//float mInitParentPositionY;				// 親の初期位置(Y)
	bool mIsMove;							// 動いているか
	bool mIsFloorHit;						// 床と当たったか
	bool mIsCloudHit;						// 雲と当たったか
	bool mIsImmortalBody;					// 不死身の敵か(未実装)
	bool mIsDistanceAct;					// 距離判定の行動をするか
	bool mIsBlowAway;						// 死亡時に吹き飛ぶかどうか
	bool mIsKnockBackDirection;				// ノックバックの方向を決めたか
	bool mIsChaseRotate;					// プレイヤーの方向を向くかどうか
	bool mIsAttckMode;						// 攻撃途中か(距離外に出た場合の攻撃中止の防止)
	bool mIsTornadoRange;					// 竜巻の範囲内か
	bool mIsTornadoBlowAway;				// 竜巻で吹き飛ばすか
	bool mIsDead;							// プレイヤーと当たったか
	// 行動配列
	typedef std::vector<EnemyState> DistanceVector;
	DistanceVector mDistanceVector;

private:
	const float mResPawnHeigth = -10.0f;	// リスポーンする高さ
};