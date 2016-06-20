#include "EnemyCG.h"

//アクターなど基本のインクルード
#include "h_standard.h"
//コンポーネント全てのインクルード
#include "h_component.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void EnemyCG::Initialize(){
	mCurrentAnimationID = 0;
	mPastAnimationID = mCurrentAnimationID;
	mAnimationTimeScale = 1.0f;
	mIsAnimationLoop = true;
	mIsAnimationChange = false;
	mIsChangeStatus = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void EnemyCG::Start(){
	//mCurrentAnimationID = mEnemyScript->GetAnimationID();
	//game->Debug()->Log(std::to_string(mCurrentAnimationID));
}

//毎フレーム呼ばれます
void EnemyCG::Update(){
	auto animation = gameObject->GetComponent<AnimationComponent>();
	// mCurrentAnimationID != mPastAnimationID
	// IDが違った場合は変更処理を行う
	if (mIsAnimationChange) {
		// アニメーションを変更
		animation->ChangeAnimetion(mCurrentAnimationID);
		// 今のアニメーションの設定を変更する
		// 仮状態で１・０の変更にする
		auto currentAnimaParam = animation->GetAnimetionParam(mCurrentAnimationID);
		//currentAnimaParam.mWeight = 1.0f;
		currentAnimaParam.mTime = 0.0f;
		currentAnimaParam.mTimeScale = 1.0f;
		// 前に使っていたアニメーションのステータスをリセットする
		auto pastAnimationParam = animation->GetAnimetionParam(mPastAnimationID);
		//pastAnimationParam.mWeight = 0.0f;
		pastAnimationParam.mTime = 0.0f;
		pastAnimationParam.mTimeScale = 1.0f;
		// 設定したステータスをセットする
		animation->SetAnimetionParam(mCurrentAnimationID, currentAnimaParam);
		animation->SetAnimetionParam(mPastAnimationID, pastAnimationParam);

		mIsAnimationChange = false;
	}

	if (mCurrentAnimationID != mPastAnimationID) {
		auto deltaTime = game->DeltaTime()->GetDeltaTime();
		auto currentAnimaParam = animation->GetAnimetionParam(mCurrentAnimationID);
		auto pastAnimationParam = animation->GetAnimetionParam(mPastAnimationID);
		// ウェイトの変更
		currentAnimaParam.mWeight += 1.0f * 10.0f * deltaTime;
		if (currentAnimaParam.mWeight > 1.0f) currentAnimaParam.mWeight = 1.0f;
		pastAnimationParam.mWeight -= 1.0f * 10.0f * deltaTime;
		if (pastAnimationParam.mWeight < 0.0f) pastAnimationParam.mWeight = 0.0f;
		// 設定したステータスをセットする
		animation->SetAnimetionParam(mCurrentAnimationID, currentAnimaParam);
		animation->SetAnimetionParam(mPastAnimationID, pastAnimationParam);
		// IDの更新
		if (currentAnimaParam.mWeight == 1.0f) mPastAnimationID = mCurrentAnimationID;
	}

	auto animaParam = animation->GetAnimetionParam(mCurrentAnimationID);
	// ステータスに変更があった場合は一回だけ変える
	if (mIsChangeStatus) {
		auto animaParam = animation->GetAnimetionParam(mCurrentAnimationID);
		animaParam.mTimeScale = mAnimationTimeScale;
		animaParam.mLoop = mIsAnimationLoop;
		animation->SetAnimetionParam(mCurrentAnimationID, animaParam);
		// ステータスを変えたら、連続して変えないようにする
		mIsChangeStatus = false;
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void EnemyCG::Finish(){
	mCurrentAnimationID = 0;
}

//コライダーとのヒット時に呼ばれます
void EnemyCG::OnCollideBegin(Actor* target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void EnemyCG::OnCollideEnter(Actor* target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void EnemyCG::OnCollideExit(Actor* target){
	(void)target;
}

// アニメーションのIDを変更します
void EnemyCG::SetAnimationID(int id) {
	if (mCurrentAnimationID != id) {
		mCurrentAnimationID = id;
		mIsAnimationChange = true;
		mIsChangeStatus = true;
	}
}

// アニメーションのタイムを変更します
void EnemyCG::SetAnimationTime(float time) {
	auto animation = gameObject->GetComponent<AnimationComponent>();
	auto currentAnimaParam = animation->GetAnimetionParam(mCurrentAnimationID);
	currentAnimaParam.mTime = time;
	animation->SetAnimetionParam(mCurrentAnimationID, currentAnimaParam);
}

// アニメーションのタイムスケールを変更します
void EnemyCG::SetTimeScale(float timeScale) {
	if (mAnimationTimeScale != timeScale) {
		mAnimationTimeScale = timeScale;
		mIsChangeStatus = true;
	}
}

// アニメーションのループを変更します
void EnemyCG::SetLoop(bool isLoop) {
	if (mIsAnimationLoop != isLoop) {
		mIsAnimationLoop = isLoop;
		mIsChangeStatus = true;
	}
}

// 現在のアニメーションのタイムを取得します
float EnemyCG::GetAnimationTime() {
	auto animation = gameObject->GetComponent<AnimationComponent>();
	auto currentAnimaParam = animation->GetAnimetionParam(mCurrentAnimationID);
	return currentAnimaParam.mTime;
}