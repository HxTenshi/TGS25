#pragma once
#include "main.h"

class SoundBox :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// サウンドの名前を設定します
	void SetSoundName(const std::string name);

private:
	//メンバ変数
	bool mIsSoundPlay;
	std::string mName;
};