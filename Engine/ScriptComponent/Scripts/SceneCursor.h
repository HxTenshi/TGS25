
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
	// �{�^���̔z��ɒǉ����܂�
	void AddButtonContainer(const char* buttonName);
	// �V�[���̔z��ɒǉ����܂�
	void AddSceneContainer(const std::string sceneName);
	// �����ꂽ�{�^���ɂ���ăV�[�����ړ����܂�
	void OnChangeScene();
	// �{�^���̒l���擾���܂�
	int GetButtonCount();
	// �V�[�����ς��������Ԃ��܂�
	bool IsChangeScene();

private:
	//�����o�ϐ�
	int mButtonCount;			// ���ݎw�肵�Ă���{�^���̒l
	int mPastButtonCount;		// �ߋ��̃{�^���̒l
	SERIALIZE
	int mSetLerpTime;				// ������Ԏ���
	SERIALIZE
	float mSpeed;
	float mLerpTime;
	SERIALIZE
	float mAddCursorPositionX;	// �J�[�\���̈ʒu�����Z����(X)
	bool mIsCursorMove;			// �J�[�\���������Ă��邩
	bool mIsChangeScene;		// �V�[�����ς������
	XMVECTOR mLerp;
	// �{�^���R���e�i
	typedef std::vector<Actor*> ButtonContainer;
	ButtonContainer mButtonContainer;
	// �V�[���R���e�i
	typedef std::vector<std::string> SceneContainer;
	SceneContainer mSceneContainer;
};