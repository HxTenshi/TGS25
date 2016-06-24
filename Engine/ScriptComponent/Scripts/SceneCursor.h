
#pragma once
#include "main.h"
#include <vector>

class Actor;

class SceneCursor :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// ボタンの配列に追加します
	void AddButtonContainer(const char* buttonName);
	// シーンの配列に追加します
	void AddSceneContainer(const std::string sceneName);
	// 押されたボタンによってシーンを移動します
	void OnChangeScene();
	// ボタンの値を取得します
	int GetButtonCount();
	// シーンが変わったかを返します
	bool IsChangeScene();

private:
	//メンバ変数
	int mButtonCount;			// 現在指定しているボタンの値
	int mPastButtonCount;		// 過去のボタンの値
	SERIALIZE
	int mSetLerpTime;				// 線分補間時間
	SERIALIZE
	float mSpeed;
	float mLerpTime;
	SERIALIZE
	float mAddCursorPositionX;	// カーソルの位置を加算する(X)
	bool mIsCursorMove;			// カーソルが動いているか
	bool mIsChangeScene;		// シーンが変わったか
	XMVECTOR mLerp;
	// ボタンコンテナ
	typedef std::vector<Actor*> ButtonContainer;
	ButtonContainer mButtonContainer;
	// シーンコンテナ
	typedef std::vector<std::string> SceneContainer;
	SceneContainer mSceneContainer;
};