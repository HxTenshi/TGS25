
#pragma once
#include "main.h"


class RetryScene :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// リスタートシーンの名前を入れます
	void SetRetrySceneName(const std::string name);
	// リスタートシーンの名前を取得します
	std::string GetRetrySceneName();

private:
	//メンバ変数
	static std::string mRetrySceneName;	// 静的なシーンの名前
};