
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
	// シーンが変わるを返します
	bool IsChangeScene();

private:
	//メンバ変数
	int mButtonCount;
	bool mIsChangeScene;
	bool mIsSetColor;
	// ボタン配列
	typedef std::vector<Actor*> ButtonContainer;
	ButtonContainer mButtonContainer;
	// 初期の色配列
	typedef std::vector<XMFLOAT4> InitColorContainer;
	InitColorContainer mInitColorContainer;
	// シーン配列
	typedef std::vector<std::string> SceneContainer;
	SceneContainer mSceneContainer;
};