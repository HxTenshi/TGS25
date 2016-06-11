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
	mChangeStatus = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void EnemyCG::Start(){
	//mCurrentAnimationID = mEnemyScript->GetAnimationID();
	//game->Debug()->Log(std::to_string(mCurrentAnimationID));
}

//毎フレーム呼ばれます
void EnemyCG::Update(){

	auto animation = gameObject->GetComponent<AnimationComponent>();

	// IDが違った場合は変更処理を行う
	if (mCurrentAnimationID != mPastAnimationID) {
		// アニメーションを変更
		animation->ChangeAnimetion(mCurrentAnimationID);
		// 各アニメーションのWeightを変更
		// 仮状態で１・０の変更にする
		auto nowAnimaParam = animation->GetAnimetionParam(mCurrentAnimationID);
		nowAnimaParam.mWeight = 1.0f;
		nowAnimaParam.mTime = 0.0f;
		nowAnimaParam.mTimeScale = 1.0f;

		auto pastAnimation = animation->GetAnimetionParam(mPastAnimationID);
		pastAnimation.mWeight = 0.0f;
		pastAnimation.mTime = 0.0f;
		pastAnimation.mTimeScale = 1.0f;

		//auto aParam = gameObject->GetComponent<AnimeParam>();

		// 設定したステータスをセットする
		animation->SetAnimetionParam(mCurrentAnimationID, nowAnimaParam);
		animation->SetAnimetionParam(mPastAnimationID, pastAnimation);

		mPastAnimationID = mCurrentAnimationID;
	}


	auto animaParam = animation->GetAnimetionParam(mCurrentAnimationID);
	// ステータスに変更があった場合は一回だけ変える
	if (mChangeStatus) {
		auto animaParam = animation->GetAnimetionParam(mCurrentAnimationID);
		animaParam.mTimeScale = mAnimationTimeScale;
		animaParam.mLoop = mIsAnimationLoop;
		//pastAnimation.mTime += 
		/*auto deltaTime = game->DeltaTime()->GetDeltaTime();
		animaParam.mTime += deltaTime;*/
		animation->SetAnimetionParam(mCurrentAnimationID, animaParam);

		mChangeStatus = false;
	}	

	//game->Debug()->Log(std::to_string(animaParam.mTime));
	//game->DeltaTime()->SetTimeScale(1.0f);
	//game->Debug()->Log(std::to_string(game->DeltaTime()->GetDeltaTime()));
	//game->Debug()->Log(std::to_string(mCurrentAnimationID));
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
		mChangeStatus = true;
	}
}

// アニメーションのタイムスケールを変更します
void EnemyCG::SetTimeScale(float timeScale) {
	if (mAnimationTimeScale != timeScale) {
		mAnimationTimeScale = timeScale;
		mChangeStatus = true;
	}
}

// アニメーションのループを変更します
void EnemyCG::SetLoop(bool isLoop) {
	if (mIsAnimationLoop != isLoop) {
		mIsAnimationLoop = isLoop;
		mChangeStatus = true;
	}
}