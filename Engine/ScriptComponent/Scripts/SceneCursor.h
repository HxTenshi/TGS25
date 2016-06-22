
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
	SERIALIZE
	float mCursorScaleX;		// カーソルの大きさ(X)
	SERIALIZE
	float mCursorScaleY;		// カーソルの大きさ(Y)
	SERIALIZE
	float mAddCursorPositionX;	// カーソルの位置を加算する(X)
	SERIALIZE
	float mAddCursorPositionY;	// カーソルの位置を加算する(Y)
	bool mIsChangeScene;		// シーンが変わったか
	Actor* mCursorRightTop;		// 右上部分のカーソルオブジェクト
	Actor* mCursorRightBottom;	// 右下部分のカーソルオブジェクト
	Actor* mCursorLeftTop;		// 左上部分のカーソルオブジェクト
	Actor* mCursorLeftBottom;	// 左下部分のカーソルオブジェクト
	// ボタンコンテナ
	typedef std::vector<Actor*> ButtonContainer;
	ButtonContainer mButtonContainer;
	// カーソルコンテナ
	typedef std::vector<Actor*> CursorContainer;
	CursorContainer mCursorContainer;
	// カーソル座標コンテナ
	typedef std::vector<XMVECTOR> CursorVelocityContainer;
	CursorVelocityContainer mCursorVelocityContainer;
	// シーンコンテナ
	typedef std::vector<std::string> SceneContainer;
	SceneContainer mSceneContainer;
};