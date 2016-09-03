
#pragma once
#include "main.h"
#include <vector>

class Actor;
class SceneCursor;
class Fade;

enum {
	TITLESELECT_NUMBER = 0,
	STAGESELECT_NUMBER = 8,
	TITLESELECT_COUNT = 3,
	STAGESELECT_COUNT = 3
};

class TitleMnager :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// タイトルのボタンを作成します
	void createButton(const int number, const int count);
	// ボタンオブジェクトの削除
	void deleteButtonObj(const char* name);

private:
	//メンバ変数
	int mTitleButtonCount;
	SERIALIZE
	float mCameraRotateSpeed;
	SERIALIZE
	float mFadeOutSecond;
	float mSceneChangeTimer;
	Actor* mCursorObj;
	Actor* mFadeOutObj;
	SceneCursor* mCursorScript;
	Fade*  mFadeOutScript;
	const int mChangeTime = 30;
	// タイトルコンテナ
	typedef std::vector<char*> selectNumberContainer;
	selectNumberContainer mSelectNumberContainer;
};