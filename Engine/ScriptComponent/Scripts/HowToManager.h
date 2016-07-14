
#pragma once
#include "main.h"

class Actor;

class HowToManager :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;
	// �Đ����郀�[�r�[��ύX���܂�
	void SetMovie(std::string name, float time);

private:
	//�����o�ϐ�
	SERIALIZE
	float mFadeOutSecond;
	//float mMovieTimer;
	//float mMovieEndTime;
	bool mIsPlay;
	bool mIsChangeScene;
	bool mIsFadeIn;
	std::string mName;
	Actor* mFadeOutObj;
};