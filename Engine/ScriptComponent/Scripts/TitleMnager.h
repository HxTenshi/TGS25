
#pragma once
#include "main.h"

class Actor;
class SceneCursor;

class TitleMnager :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;

private:
	//�����o�ϐ�
	SERIALIZE
	float cameraRotateSpeed;
	Actor* mCursorObj;
	SceneCursor* mCursorScript;
};