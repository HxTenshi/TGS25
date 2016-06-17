
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
	// �V�[�����ς���Ԃ��܂�
	bool IsChangeScene();

private:
	//�����o�ϐ�
	int mButtonCount;
	bool mIsChangeScene;
	bool mIsSetColor;
	// �{�^���z��
	typedef std::vector<Actor*> ButtonContainer;
	ButtonContainer mButtonContainer;
	// �����̐F�z��
	typedef std::vector<XMFLOAT4> InitColorContainer;
	InitColorContainer mInitColorContainer;
	// �V�[���z��
	typedef std::vector<std::string> SceneContainer;
	SceneContainer mSceneContainer;
};